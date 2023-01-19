#include <server.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>


#define PORT 8080

using namespace server;

void Server::start()
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT));

    while (running.load())
    {

        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);

        handleAccept(acceptor, socket);
    }
}

void Server::handleAccept(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::ip::tcp::socket &socket)
{
    if (running.load())
    {
        std::thread t(&Server::handleClient, this, std::move(socket));
        t.detach();
    }
    else
    {
        acceptor.cancel();
    }
}

void Server::handleClient(boost::asio::ip::tcp::socket socket)
{
    std::cout << "New client connected" << std::endl;
    auto player = std::make_shared<shared::Player>();
    player->setSocket(socket);

    boost::asio::streambuf request;
    try
    {
        boost::asio::read_until(player->getSocket(), request, '\n');
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        player->disconnectPlayer();
        return;
    }
    std::istream requestStream(&request);
    std::string gameId;
    std::string username;
    std::shared_ptr<GameEngine> game = nullptr;
    requestStream >> gameId >> username;
    player->setUsername(username);

    // New game
    if (gameId.compare("new") == 0)
    {
        player->setUsername(username);
        game = createNewGame(player);
    }
    // Connection to existing game
    else
    {
        game = getGameById(gameId);
        bool playerConnected = false;
        if (game != nullptr)
        {
            playerConnected = connectPlayerToGame(player, game);
        }

        if (!playerConnected)
        {
            std::string response = "Error: game not available\n";
            boost::asio::write(player->getSocket(), boost::asio::buffer(response));
            player->disconnectPlayer();
            return;
        }
    }

    std::string response = "connected " + game->getTime() + " server Game ID: " + game->getId() + "\n";
    boost::asio::write(player->getSocket(), boost::asio::buffer(response));
    
    while (player->connectedToSocket.load())
    {
        boost::asio::streambuf receiveBuffer;
        boost::system::error_code error;
        std::size_t bytesTransferred{0};

        try
        {
            std::lock_guard<std::mutex> lock(player->socketReadMutex);
            bytesTransferred = boost::asio::read_until(player->getSocket(), receiveBuffer, '\n', error);
        }
        catch (const boost::system::system_error &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        if (error == boost::asio::error::operation_aborted || error == boost::asio::error::eof)
        {
            player->disconnectPlayer();
            continue;
        }
        else if (error)
        {
            std::cerr << "Error: " << error.message() << std::endl;
            player->disconnectPlayer();
            continue;
        }
        if (bytesTransferred)
        {
            processMessage(receiveBuffer, player, game);
            receiveBuffer.consume(receiveBuffer.size());
        }
    }
    std::cout << "Player " << player->getName() << " disconnected" << std::endl;
}

void Server::processMessage(boost::asio::streambuf& receiveBuffer, std::shared_ptr<shared::Player> player, std::shared_ptr<GameEngine> game)
{
    std::istream receiveStream(&receiveBuffer);
    std::string messageReceived;
    while(std::getline(receiveStream, messageReceived))
    {
        if (messageReceived.size() == 0)
        {
            continue;
        }
        else if (messageReceived.find("response") == 0)
        {
            registerClientAnswer(messageReceived, player);
        }
        else if (messageReceived.find("binary") == 0) // binary reception
        {
            size_t size = std::stoi(messageReceived.substr(8));
            std::string data = binary.receive(player, size);
            registerClientAnswer(data, player);
        }
        else
        {
            game->processClientRequest(messageReceived, player);
        }
    }

}

std::shared_ptr<GameEngine> Server::getGameById(std::string gameId)
{
    std::lock_guard<std::mutex> lock(gamesMutex);
    for (auto &g : games)
    {
        if (g->getId() == gameId)
        {
            return g;
        }
    }
    return nullptr;
}

bool Server::connectPlayerToGame(std::shared_ptr<shared::Player> player, std::shared_ptr<GameEngine> game)
{
    // check if a player with the same username already exists in the game and is in disconnected state
    for (auto &p : game->getPlayers())
    {
        if (p.get() == player.get())
        {
            p->setSocket(player->getSocket());
            player.swap(p);
            std::cout << "Player " << player->getName() << " reconnected to game " << game->getId() << std::endl;
            return true;
        }
    }
    bool res = game->addPlayer(player);
    if (!res)
    {
        player->disconnectPlayer();
        return false;
    }
    else
    {
        std::cout << "Player " << player->getName() << " connected to game " << game->getId() << std::endl;
        std::string newUserConnected = "chat " + game->getTime() + " " + "server" + " " + player->getName() + " join the game\n";
        game->sendToEveryone(newUserConnected);
        return true;
    }
}

std::shared_ptr<GameEngine> Server::createNewGame(std::shared_ptr<shared::Player> player)
{
    auto game = std::make_shared<GameEngine>(games, player);

    std::cout << "Player " << player->getName() << " created game " << game->getId() << std::endl;

    std::lock_guard<std::mutex> lock(gamesMutex);
    games.push_back(game);
    return game;
}

void Server::registerClientAnswer(const std::string &response, std::shared_ptr<shared::Player> player)
{
    std::lock_guard<std::mutex> lock(player->qAndA.sharedDataMutex);
    player->qAndA.answer = response;
    player->qAndA.answerReady = true;
    player->qAndA.condition.notify_one();
}
#include <server.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

#define PORT 8080

namespace server
{

    void Server::start()
    {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT));

        while (true)
        {

            boost::asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::thread t(&Server::handleClient, this, std::move(socket));
            t.detach();
        }
    }

    void Server::handleClient(boost::asio::ip::tcp::socket socket)
    {
        std::cout << "New client connected" << std::endl;
        auto player = std::make_shared<shared::Player>();
        player->setSocket(socket);

        boost::asio::streambuf request;
        try {
            boost::asio::read_until(player->getSocket(), request, '\n');
        } catch (const boost::system::system_error &e) {
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
            if (game != nullptr)
            {
                connectPlayerToGame(player, game);
            }

            if (player->state == shared::PlayerState::Disconnected || player->state == shared::PlayerState::WaitingForGame)
            {
                std::string response = "Error: game not available\n";
                boost::asio::write(player->getSocket(), boost::asio::buffer(response));
                return;
            }
        }

        std::string response = "OK\n";
        boost::asio::write(player->getSocket(), boost::asio::buffer(response));
        player->state = shared::PlayerState::Connected;

        while (player->state == shared::PlayerState::Connected)
        {
            boost::asio::streambuf receiveBuffer;
            boost::system::error_code error;
            std::size_t bytesTransferred {0};

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
                std::string messageReceived(
                    boost::asio::buffers_begin(receiveBuffer.data()),
                    boost::asio::buffers_end(receiveBuffer.data()));
                receiveBuffer.consume(receiveBuffer.size());

                if (messageReceived.find("response") == 0)
                {
                    registerClientAnswer(messageReceived, player);
                }
                else
                {
                    game->processClientRequest(messageReceived, player);
                }
            }
        }
        std::cout << "Client disconnected" << std::endl;
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

    void Server::connectPlayerToGame(std::shared_ptr<shared::Player> player, std::shared_ptr<GameEngine> game)
    {
        // check if a player with the same username already exists in the game and is in disconnected state
        for (auto &p : game->getPlayers())
        {
            if (p.get() == player.get())
            {
                p->setSocket(player->getSocket());
                player.swap(p);
                return;
            }
        }
        bool res = game->addPlayer(player);
        if (!res)
        {
            // TODO : a changer (appeler disconnect ?)
            player->state = shared::PlayerState::Disconnected;
        }
    }

    std::shared_ptr<GameEngine> Server::createNewGame(std::shared_ptr<shared::Player> player)
    {
        auto game = std::make_shared<GameEngine>(games, player);

        game->addPlayer(player);
        std::cout << "New game created with id: " << game->getId() << std::endl;

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

}

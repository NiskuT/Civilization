#include <server.hpp>
#include <boost/asio.hpp>

#define PORT 8080

namespace server
{

    void Server::start()
    {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT));

        while (true)
        {

            boost::asio::ip::tcp::socket socket(io_service);
            acceptor.accept(socket);

            std::thread t(&Server::handleClient, this, std::move(socket));
            t.detach();
        }
    }

    void Server::handleClient(boost::asio::ip::tcp::socket socket)
    {
        auto player = std::make_shared<shared::Player>(socket);

        // Attente de la demande du client pour rejoindre une partie
        boost::asio::streambuf request;
        boost::asio::read_until(player->getSocket(), request, "\n");
        std::istream requestStream(&request);
        std::string gameId;
        std::string username;
        requestStream >> gameId >> username;
        player->setUsername(username);

        // New game
        if (gameId.compare("new") == 0)
        {
            player->setUsername(username);
            createNewGame(player);
        }
        // Connection to existing game
        else
        {
            GameEngine *game = getGameById(gameId);
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

        // Envoi de la confirmation au client
        std::string response = "OK\n";
        boost::asio::write(player->getSocket(), boost::asio::buffer(response));

        // Attente des prochaines actions du client
        while (true)
        {
            // Lecture de l'action envoyée par le client
            boost::asio::streambuf request;
            boost::asio::read_until(player->getSocket(), request, "\n");

            // Traitement de l'action
            // ...

            // Envoi de la réponse au client
            // ...
        }
    }

    GameEngine *Server::getGameById(std::string gameId)
    {
        GameEngine *game = nullptr;
        std::lock_guard<std::mutex> lock(gamesMutex);
        for (auto &g : games)
        {
            if (g->getId() == gameId)
            {
                game = g.get();
                break;
            }
        }
        return game;
    }

    void Server::connectPlayerToGame(std::shared_ptr<shared::Player> player, GameEngine *game)
    {
        // check if a player with the same username already exists in the game and is in disconnected state
        bool reconnected = false;
        for (auto &p : game->getPlayers())
        {
            if (p == *player)
            {
                p->state = shared::PlayerState::Connected;
                p->socket = std::move(player->getSocket());
                reconnected = true;
                break;
            }
        }
        if (!reconnected)
        {
            bool res = game->addPlayer(player);
            if (!res)
            {
                player->state = shared::PlayerState::Disconnected;
            }
        }
    }

    void Server::createNewGame(std::shared_ptr<shared::Player> player)
    {
        // Création d'une nouvelle partie avec un ID unique
        auto game = std::make_shared<GameEngine>(games);

        // Ajout du joueur à la partie
        game->addPlayer(player);

        // Ajout de la partie à la liste des parties en cours
        std::lock_guard<std::mutex> lock(gamesMutex);
        games.push_back(game);
    }
}
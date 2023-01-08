#include <server.hpp>
#include <algorithm>
#include <random>

namespace server
{
    GameEngine::GameEngine(std::vector<std::shared_ptr<GameEngine>> &games, std::shared_ptr<shared::Player> player)
    {
        gameId = GameEngine::generateRandomId(games);
        players.push_back(player);
    }

    bool GameEngine::addPlayer(std::shared_ptr<shared::Player> player)
    {
        if (isPublic && (players.size() < 4))
        {
            players.push_back(player);
        }
        else
        {
            return false;
        }

        if (players.size() == 4)
        {
            state = GameState::Running;
        }
        return true;
    }

    std::vector<std::shared_ptr<shared::Player>> &GameEngine::getPlayers()
    {
        return players;
    }

    static std::string generateRandomId(const std::vector<std::shared_ptr<GameEngine>> &games)
    {
        static const std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        static std::mt19937 generator(std::random_device{}());
        static std::uniform_int_distribution<std::string::size_type> distribution(0, alphabet.size() - 1);

        std::string id;
        do
        {
            id.resize(6);
            std::generate(id.begin(), id.end(), [&]
                          { return alphabet[distribution(generator)]; });
        } while (std::any_of(games.begin(), games.end(), [&](const auto &game)
                             { return game->getId() == id; }));
        return id;
    }

    std::string GameEngine::getId()
    {
        return gameId;
    }

    void GameEngine::processClientRequest(std::string requestString, std::shared_ptr<shared::Player> player)
    {
        std::vector<std::string> requestComponents = splitString(requestString, ' ');
        if (requestComponents.size() < 1)
        {
            return;
        }

        std::string command = requestComponents[0];

        std::string response;
        if (command.compare("move") == 0)
        {
            response = "OK\n";
        }
        else if (command.compare("use_item") == 0)
        {
            response = "OK\n";
        }
        else if (command.compare("disconnect") == 0)
        {
            player->disconnectPlayer();
            response = "OK\n";
        }
        else
        {
            response = "Error: invalid command\n";
        }

        {
            std::lock_guard<std::mutex> lock(player->socketWriteMutex);
            boost::asio::write(player->getSocket(), boost::asio::buffer(response));
        }
    }

    static std::vector<std::string> splitString(std::string str, char delimiter)
    {
        std::vector<std::string> components;
        std::stringstream ss(str);
        std::string component;
        while (std::getline(ss, component, delimiter))
        {
            components.push_back(component);
        }
        return components;
    }

    void askClient(std::shared_ptr<shared::Player> player)
    {
        player->qAndA.answerReady = false;
        {
            std::lock_guard<std::mutex> lock(player->socketWriteMutex);
            boost::asio::write(player->getSocket(), boost::asio::buffer(player->qAndA.question));
        }

        std::unique_lock<std::mutex> responseLock(player->qAndA.sharedDataMutex);
        player->qAndA.condition.wait(responseLock, [&]
                                     { return player->qAndA.answerReady; });

        std::string response = player->qAndA.answer;
        player->qAndA.answerReady = false;
    }

}

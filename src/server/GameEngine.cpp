#include <server.hpp>
#include <algorithm>
#include <random>

namespace server
{
    GameEngine::GameEngine(shared::Player &player)
    {
        gameId = generateRandomId(games);
    }

    bool GameEngine::addPlayer(std::shared_ptr<shared::Player> player)
    {


        if (isPublic && (players.size() < 4))
        {
            players.push_back(player);
        }
        else {
            return false;
        }

        if (players.size() == 4)
        {
            state = GameState::Started;
        }
        return true;
    }

    std::vector<std::shared_ptr<shared::Player>> GameEngine::getPlayers()
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
                             { return game->gameId == id; }));
        return id;
    }

    std::string GameEngine::getId()
    {
        return gameId;
    }


}



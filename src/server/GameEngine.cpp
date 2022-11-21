#include <server.hpp>

namespace server {
    GameEngine::GameEngine(shared::Player &player) {
        gameId = "azerty";
        listOfPlayer.push_back(player);
    }
}
#include <server.hpp>

namespace server {
    GameEngine::GameEngine(shared::Player &player) {
        isPublic = True;
        gameId = 0;
        listOfPlayer.push_back(player);
    }
}
#include <server.hpp>

namespace server {


/**
 * @brief This function is used to create a new game
 * @param player The player who create the game 
 * @return 
 */
void Server::acceptClient(shared::Player &player) {
    listOfGame.emplace_back(player);
    
}

void Server::openGame() {
    // TODO implement here
}

void Server::connectToGame(std::string id, shared::Player &player) {
    for (auto &game : listOfGame) {
        if (game.isPublic 
                && (game.listOfPlayer.size() < 4)
                && (game.gameId.compare(id) == 0)) {
            game.listOfPlayer.push_back(player);
            return;
        }
            
    }
}

}
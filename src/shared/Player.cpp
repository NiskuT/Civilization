#include <shared.hpp>


namespace shared {

Player::Player(std::string username) {
    this->username = username;
    for (int i = 1; i < 4; i++) {
        this->resource[i] = 0;
        this->wonderRessources[i] = 0;
    }
}

std::string Player::getName() {
    return this->username;
}


Caravan * Player::getCaravanAt(int posX, int posY) {
    for (int i = 0; i < (int) this->listOfCaravan.size(); i++) {
        if (this->listOfCaravan[i]->getPosX() == posX && this->listOfCaravan[i]->getPosY() == posY) {
            return this->listOfCaravan[i];
        }
    }
    return NULL;
}

}
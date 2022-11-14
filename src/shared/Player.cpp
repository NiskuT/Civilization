#include <shared.hpp>


namespace shared {

Player::Player() {
    this->ressources = {0,0,0,0};
    this->wonderRessources = {0,0,0,0};
}

std::string Player::getName() {
    return this->name;
}



}
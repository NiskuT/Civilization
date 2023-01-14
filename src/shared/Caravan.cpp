#include <shared.hpp>

namespace shared {
    Caravan::Caravan(int posX, int posY) {
        this->posX = posX;
        this->posY = posY;
    }

    int Caravan::getPosX() {
        return this->posX;
    }

    int Caravan::getPosY() {
        return this->posY;
    }

    void Caravan::setPos(int posX, int posY) {
        this->posX = posX;
        this->posY = posY;
    }

    void Caravan::move(std::vector<std::array<int,2>> pathToFollow) {
        for (int i = 0; i < (int) pathToFollow.size(); i++) {
            this->posX = pathToFollow.at(i)[0];
            this->posY = pathToFollow.at(i)[1];
        }
    }

}
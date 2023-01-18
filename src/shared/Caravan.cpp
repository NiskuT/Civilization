#include <shared.hpp>

namespace shared {
    Caravan::Caravan(std::array<unsigned, 2> position) {
        this->position = position;
    }


    std::array<unsigned, 2> Caravan::getPosition() {
        return this->position;
    }

    void Caravan::setPos(std::array<unsigned, 2> position) {
        this->position = position;
    }

    void Caravan::setUsed(bool used) {
        this->used = used;
    }

    bool Caravan::isUsed() {
        return this->used;
    }
}

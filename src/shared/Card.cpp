#include <shared.hpp>

namespace shared
{
    Card::Card(CardsEnum type)
    {
        this->type = type;
        this->level = 1;
        this->numberOfBox = 15;
    }

    CardsEnum Card::getType()
    {
        return this->type;
    }

    unsigned Card::getLevel()
    {
        return this->level;
    }

    void Card::addLevel()
    {
        this->level += 1;
    }

    unsigned Card::getNumberOfBox()
    {
        return this->numberOfBox;
    }

    void Card::addBox(unsigned numberOfBoxToAdd)
    {
        this->numberOfBox += numberOfBoxToAdd;
    }

    void Card::removeBox(unsigned numberOfBoxToRemove)
    {
        this->numberOfBox -= numberOfBoxToRemove;
    }

}
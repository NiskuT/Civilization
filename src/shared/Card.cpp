#include <shared.hpp>

using namespace shared;
Card::Card(CardsEnum type)
{
    this->type = type;
    this->level = 1;
    this->dificulty = 1;
    this->numberOfBox = 0;
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

void Card::deleteBox(unsigned numberOfBoxToDelete)
{
    this->numberOfBox -= numberOfBoxToDelete;
}

unsigned Card::getDificulty()
{
    return this->dificulty;
}

void Card::setDificulty(unsigned newDificulty)
{
    this->dificulty = newDificulty;
}

#include <shared.hpp>

using namespace shared;

Caravan::Caravan()
{
    this->position = {0, 0};
    this->used = false;
}

Caravan::Caravan(std::array<unsigned, 2> position, std::string player)
{
    this->position = position;
    this->player = player;
}

std::array<unsigned, 2> Caravan::getPosition()
{
    return this->position;
}

void Caravan::setPos(std::array<unsigned, 2> position)
{
    this->position = position;
}

void Caravan::setUsed(bool used)
{
    this->used = used;
}

bool Caravan::isUsed()
{
    return this->used;
}

ElementEnum Caravan::getType()
{
    return ElementEnum::caravan;
}

std::string Caravan::getPlayerName()
{
    return this->player;
}

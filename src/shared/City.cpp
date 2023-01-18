#include <shared.hpp>

using namespace shared;

City::City(std::array<unsigned, 2> position)
{
    this->position = position;
    this->isMature = false;
    this->isCapital = false;
    this->isStateCity = false;
}

std::array<unsigned, 2> City::getPosition()
{
    return this->position;
}

ElementEnum City::getType()
{
    return ElementEnum::city;
}

#include <shared.hpp>

using namespace shared;

City::City()
{
    this->isMature = false;
    this->isCapital = false;
    this->isStateCity = false;
    this->position = {0, 0};
}

City::City(std::array<unsigned, 2> position,std::string player)
{
    this->position = position;
    this->isMature = false;
    this->isCapital = false;
    this->isStateCity = false;
    this->player = player;
}

std::array<unsigned, 2> City::getPosition()
{
    return this->position;
}

ElementEnum City::getType()
{
    return ElementEnum::city;
}

void City::setStateCity(CityStateEnum name)
{
    isStateCity = true;
    stateCityType = name;
}

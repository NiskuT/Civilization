#include <shared.hpp>

using namespace shared;

ControlPawn::ControlPawn()
{
    this->reinforced = false;
    this->position = {0, 0};
}

ControlPawn::ControlPawn(std::array<unsigned, 2> position, std::string player)
{
    this->reinforced = false;
    this->position = position;
    this->player = player;
}

bool ControlPawn::isReinforced()
{
    return this->reinforced;
}

void ControlPawn::setReinforced()
{
    this->reinforced = true;
}

void ControlPawn::setNotReinforced()
{
    this->reinforced = false;
}

std::array<unsigned, 2> ControlPawn::getPosition()
{
    return this->position;
}

ElementEnum ControlPawn::getType()
{
    return ElementEnum::controlPawn;
}

std::string ControlPawn::getPlayerName()
{
    return this->player;
}

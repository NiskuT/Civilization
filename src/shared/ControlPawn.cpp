#include <shared.hpp>

using namespace shared;

ControlPawn::ControlPawn(std::array<unsigned, 2> position)
{
    this->reinforced = false;
    this->position = position;
}

bool ControlPawn::isReinforced()
{
    return this->reinforced;
}

void ControlPawn::setReinforced()
{
    this->reinforced = true;
}

std::array<unsigned, 2> ControlPawn::getPosition()
{
    return this->position;
}

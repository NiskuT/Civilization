#include <shared.hpp>

using namespace shared;

ControlPawn::ControlPawn()
{
    this->reinforced = false;
    this->position = {0, 0};
}

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

ElementEnum ControlPawn::getType()
{
    return ElementEnum::controlPawn;
}

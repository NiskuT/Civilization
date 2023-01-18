#include <shared.hpp>

using namespace shared;

ControlPawn::ControlPawn()
{
    this->reinforced = false;
}

bool ControlPawn::isReinforced()
{
    return this->reinforced;
}

void ControlPawn::setReinforced()
{
    this->reinforced = true;
}

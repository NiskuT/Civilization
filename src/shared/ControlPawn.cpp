#include <shared.hpp>

using namespace shared;

bool ControlPawn::isReinforced()
{
    return this->reinforced;
}

void ControlPawn::setReinforced()
{
    this->reinforced = true;
}

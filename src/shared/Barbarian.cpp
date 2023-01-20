#include <shared.hpp>

using namespace shared;

void Barbarian::kill()
{
    this->isAlive = false;
}

ElementEnum Barbarian::getType()
{
    return ElementEnum::barbarian;
}

Barbarian::Barbarian()
{
    this->isAlive = true;
}

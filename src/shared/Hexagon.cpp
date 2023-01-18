#include <shared.hpp>

using namespace shared;

FieldLevel Hexagon::getFieldLevel()
{
    return this->level;
}

void Hexagon::setFieldType(FieldLevel field)
{
    this->level = field;
}
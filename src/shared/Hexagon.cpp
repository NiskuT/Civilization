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

std::vector<std::shared_ptr<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>> Hexagon::getElements()
{
    return elementsList;
}

void Hexagon::addElement(std::shared_ptr<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>> element)
{
    elementsList.push_back(element);
}

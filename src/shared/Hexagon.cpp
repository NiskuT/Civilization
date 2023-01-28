#include <shared.hpp>

using namespace shared;

Hexagon::Hexagon()
{
    this->level = FieldLevel::Grassland;
}

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

void Hexagon::removeElement(std::shared_ptr<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>> elementToRemove)
{
    for (auto element : elementsList)
    {
        if (std::holds_alternative<Caravan>(*elementToRemove) && std::holds_alternative<Caravan>(*element))
        {
            if (std::get<Caravan>(*elementToRemove).getPlayerName() == std::get<Caravan>(*element).getPlayerName())
            {
                elementsList.erase(std::remove(elementsList.begin(), elementsList.end(), element), elementsList.end());
                break;
            }
        }
        else if (std::holds_alternative<Barbarian>(*elementToRemove) && std::holds_alternative<Barbarian>(*element))
        {
            elementsList.erase(std::remove(elementsList.begin(), elementsList.end(), element), elementsList.end());
            break;
        }
        else if (std::holds_alternative<BarbarianVillage>(*elementToRemove) && std::holds_alternative<BarbarianVillage>(*element))
        {
            elementsList.erase(std::remove(elementsList.begin(), elementsList.end(), element), elementsList.end());
            break;
        }
        else if (std::holds_alternative<ControlPawn>(*elementToRemove) && std::holds_alternative<ControlPawn>(*element))
        {
            if (std::get<ControlPawn>(*elementToRemove).getPlayerName() == std::get<ControlPawn>(*element).getPlayerName())
            {
                elementsList.erase(std::remove(elementsList.begin(), elementsList.end(), element), elementsList.end());
                break;
            }
        }
        else if (std::holds_alternative<City>(*elementToRemove) && std::holds_alternative<City>(*element))
        {
            if (std::get<City>(*elementToRemove).getPlayerName() == std::get<City>(*element).getPlayerName())
            {
                elementsList.erase(std::remove(elementsList.begin(), elementsList.end(), element), elementsList.end());
                break;
            }
        }
    }
}

void Hexagon::clearElement()
{
    elementsList.clear();
}

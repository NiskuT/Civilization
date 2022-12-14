#include <shared.hpp>

namespace shared
{
    FieldLevel Hexagon::getFieldLevel()
    {
        return this->level;
    }

    void Hexagon::setFieldType(FieldLevel field)
    {
        this->level = field;
    }

    bool Hexagon::getElements(std::vector<Element*>* fieldElement)
    {
        if (fieldElement == NULL)
        {
            return false;
        }
        else
        {
            *fieldElement = this->listElements;
            return true;
        }
    }
    
}
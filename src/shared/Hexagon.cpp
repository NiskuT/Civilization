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

    std::shared_ptr<ElementPtr> Hexagon::getElement(ElementEnum elementType)
    {
        for (std::shared_ptr<Element> element : this->elementsList)
        {
            if (element->getType() == elementType)
            {
                return element->getPtr();
            }
        }
        return nullptr;
    }
}
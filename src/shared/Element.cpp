#include <shared.hpp>

namespace shared
{
    ElementEnum Element::getType()
    {
        return this->element_obj->getType();
    }
}
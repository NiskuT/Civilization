#include <shared.hpp>

namespace shared
{
    ElementEnum Element::getType()
    {
        return this->type;
    }

    std::shared_ptr<ElementPtr> Element::getPtr()
    {
        return this->ptr;
    }
}
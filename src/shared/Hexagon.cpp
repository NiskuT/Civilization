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

    std::vector<std::shared_ptr<Element>>& Hexagon::getElements ()
    {
        return this->listElements;
    }

    template <class Archive>
    void Hexagon::serialize(Archive &ar, const unsigned int version)
    {
        ar &listElements;
        ar &level;
    }

}
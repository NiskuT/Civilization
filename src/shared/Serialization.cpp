#include <shared.hpp>
#include <boost/serialization/shared_ptr.hpp>

namespace shared
{
    template <class Archive>
    void Map::serialize(Archive &ar, const unsigned int version)
    {
        ar &mapOfTheGame;
        ar &height;
        ar &width;
    }

    template <class Archive>
    void Hexagon::serialize(Archive &ar, const unsigned int version)
    {
        ar &level;
    }
}
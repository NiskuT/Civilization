#include <shared.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>

using namespace shared;

template <class Archive>
void Map::serialize(Archive &ar, const unsigned int version)
{
    boost::serialization::split_member(ar, *this, version);
}

template <class Archive>
void Map::save(Archive &ar, const unsigned int version) const
{
    ar << height;
    ar << width;
    ar << mapOfTheGame;
}

template <class Archive>
void Map::load(Archive &ar, const unsigned int version)
{
    ar >> height;
    ar >> width;
    this->init();
    ar >> mapOfTheGame;
}

template <class Archive>
void Hexagon::serialize(Archive &ar, const unsigned int version)
{
    ar &level;
}

#include <shared.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>

using namespace shared;

/*!
 * @brief Quentin
 * @param version
 * @param ar
 */
template <class Archive>
void Map::serialize(Archive &ar, const unsigned int version)
{
    boost::serialization::split_member(ar, *this, version);
}

/*!
 * @brief Quentin
 * @param ar
 * @param version
 */
template <class Archive>
void Map::save(Archive &ar, const unsigned int version) const
{
    ar << height;
    ar << width;
    ar << mapOfTheGame;
    /*for (auto &i : mapOfTheGame)
    {
        ar << *i;
    }*/
}

/*!
 * @brief Quentin
 * @param ar
 * @param version
 */
template <class Archive>
void Map::load(Archive &ar, const unsigned int version)
{
    ar >> height;
    ar >> width;
    isInizialize = false;
    this->init();
    ar >> mapOfTheGame;
    /*for (auto &i : mapOfTheGame)
    {
        ar >> *i;
    }*/
}

/*!
 * @brief Quentin
 * @param ar
 * @param version
 */
template <class Archive>
void Hexagon::serialize(Archive &ar, const unsigned int version)
{
    ar &level;
}

#include <shared.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/variant.hpp>
#include <algorithm>

using namespace shared;

typedef std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City> elementVariant;

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
    isInizialize = false;
    this->init();
    ar >> mapOfTheGame;
}

template <class Archive>
void Hexagon::serialize(Archive &ar, const unsigned int version)
{
    boost::serialization::split_member(ar, *this, version);
}

template <class Archive>
void Hexagon::save(Archive &ar, const unsigned int version) const
{
    ar << level;
    ar << elementsList.size();
    ar << elementsList;
}

template <class Archive>
void Hexagon::load(Archive &ar, const unsigned int version)
{
    ar >> level;
    size_t size = 0;
    ar >> size;
    std::generate_n(std::back_inserter(elementsList), size, []()
                    { return std::make_shared<elementVariant>(); });
    ar >> elementsList;
}

template <class Archive>
void RuleArgsStruct::serialize(Archive &ar, const unsigned int version)
{
    ar &ruleId;
    ar &numberOfBoxUsed;
    ar &caravanMovementPath;
    ar &resourceToGet;
    ar &cardToGetABox;
    ar &positionToNuke;
    ar &pawnsPositions;
    ar &militaryCardAttack;
    ar &industryCardBuildWonder;
    ar &positionOfWonder;
    ar &positionOfCity;
    ar &cardsToImprove;
}

template <class Archive>
void City::serialize(Archive &ar, const unsigned int version)
{
    ar &isStateCity;
    ar &isCapital;
    ar &isMature;
    ar &position;
}

template <class Archive>
void ControlPawn::serialize(Archive &ar, const unsigned int version)
{
    ar &position;
    ar &reinforced;
}

template <class Archive>
void BarbarianVillage::serialize(Archive &ar, const unsigned int version)
{
    // serialize the related barbarian
}

template <class Archive>
void Barbarian::serialize(Archive &ar, const unsigned int version)
{
    ar &isAlive;
}

template <class Archive>
void Caravan::serialize(Archive &ar, const unsigned int version)
{
    ar &position;
    ar &used;
}

namespace boost
{
    namespace serialization
    {

        template <class Archive, typename... Ts>
        void serialize(Archive &ar, std::variant<Ts...> &var, const unsigned int version)
        {
            std::visit([&](auto &&value)
                       { ar &value; },
                       var);
        }

    }
}
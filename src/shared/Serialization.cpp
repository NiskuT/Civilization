#include <shared.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/variant.hpp>
#include <algorithm>
#include <iostream>

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

    std::vector<Caravan> caravans;
    std::vector<Barbarian> barbarians;
    std::vector<BarbarianVillage> barbarianVillages;
    std::vector<ControlPawn> controlPawns;
    std::vector<City> cities;

    for (const auto &element : elementsList)
    {
        switch (element->index())
        {
        case 0:
            caravans.push_back(std::get<Caravan>(*element));
            break;
        case 1:
            barbarians.push_back(std::get<Barbarian>(*element));
            break;
        case 2:
            barbarianVillages.push_back(std::get<BarbarianVillage>(*element));
            break;
        case 3:
            controlPawns.push_back(std::get<ControlPawn>(*element));
            break;
        case 4:
            cities.push_back(std::get<City>(*element));
            break;
        }
    }
    ar << caravans.size();
    ar << barbarians.size();
    ar << barbarianVillages.size();
    ar << controlPawns.size();
    ar << cities.size();
    ar << caravans;
    ar << barbarians;
    ar << barbarianVillages;
    ar << controlPawns;
    ar << cities;
}

template <class Archive>
void Hexagon::load(Archive &ar, const unsigned int version)
{
    ar >> level;
    size_t size = 0;

    std::vector<Caravan> caravans;
    std::vector<Barbarian> barbarians;
    std::vector<BarbarianVillage> barbarianVillages;
    std::vector<ControlPawn> controlPawns;
    std::vector<City> cities;

    ar >> size;
    std::generate_n(std::back_inserter(caravans), size, []()
                    { return Caravan(); });
    ar >> size;
    std::generate_n(std::back_inserter(barbarians), size, []()
                    { return Barbarian(); });
    ar >> size;
    std::generate_n(std::back_inserter(barbarianVillages), size, []()
                    { return BarbarianVillage(); });
    ar >> size;
    std::generate_n(std::back_inserter(controlPawns), size, []()
                    { return ControlPawn(); });
    ar >> size;
    std::generate_n(std::back_inserter(cities), size, []()
                    { return City(); });

    ar >> caravans;
    ar >> barbarians;
    ar >> barbarianVillages;
    ar >> controlPawns;
    ar >> cities;

    for (auto &caravan : caravans)
    {
        elementsList.push_back(std::make_shared<elementVariant>(caravan));
    }
    for (auto &barbarian : barbarians)
    {
        elementsList.push_back(std::make_shared<elementVariant>(barbarian));
    }
    for (auto &barbarianVillage : barbarianVillages)
    {
        elementsList.push_back(std::make_shared<elementVariant>(barbarianVillage));
    }
    for (auto &controlPawn : controlPawns)
    {
        elementsList.push_back(std::make_shared<elementVariant>(controlPawn));
    }
    for (auto &city : cities)
    {
        elementsList.push_back(std::make_shared<elementVariant>(city));
    }
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
    ar &player;
}

template <class Archive>
void ControlPawn::serialize(Archive &ar, const unsigned int version)
{
    ar &position;
    ar &reinforced;
    ar &player;
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
    ar &player;
}

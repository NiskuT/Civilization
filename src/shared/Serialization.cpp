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
    ar << elementsList.size();

    for (auto &element : elementsList)
    {
        ar << element->index();
        switch (element->index())
        {
        case 0:
            ar << std::get<Caravan>(*element);
            break;
        case 1:
            ar << std::get<Barbarian>(*element);
            break;
        case 2:
            ar << std::get<BarbarianVillage>(*element);
            break;
        case 3:
            ar << std::get<ControlPawn>(*element);
            break;
        case 4:
            ar << std::get<City>(*element);
            break;
        }
    }
}

template <class Archive>
void Hexagon::load(Archive &ar, const unsigned int version)
{
    ar >> level;
    size_t size = 0;
    ar >> size;
    /*
    std::generate_n(std::back_inserter(elementsList), size, []()
                    { return std::make_shared<elementVariant>(); });
    ar >> elementsList;*/
    for (size_t i = 0; i < size; i++)
    {
        int index = 0;
        ar >> index;
        if (index == 0)
        {
            Caravan caravan;
            ar >> caravan;
            elementsList.push_back(std::make_shared<elementVariant>(caravan));
        }
        else if (index == 1)
        {
            Barbarian barbarian;
            ar >> barbarian;
            elementsList.push_back(std::make_shared<elementVariant>(barbarian));
        }
        else if (index == 2)
        {
            BarbarianVillage barbarianVillage;
            ar >> barbarianVillage;
            elementsList.push_back(std::make_shared<elementVariant>(barbarianVillage));
        }
        else if (index == 3)
        {
            ControlPawn controlPawn;
            ar >> controlPawn;
            elementsList.push_back(std::make_shared<elementVariant>(controlPawn));
        }
        else if (index == 4)
        {
            City city;
            ar >> city;
            elementsList.push_back(std::make_shared<elementVariant>(city));
        }
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

#include <shared.hpp>
#include <math.h>
#include <ctime>
#include <variant>
#include <iostream>

#define NUMBER_OF_FIELDS 5
#define PERLIN_NOISE_WIDTH 10
#define PERLIN_NOISE_PARAM 0.8

using namespace shared;

typedef std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City> variantElement;

const FieldLevel wonderList[] = {FieldLevel::WonderEverest, FieldLevel::WonderGalapagos, FieldLevel::WonderKilimanjaro,
                                 FieldLevel::WonderMessa, FieldLevel::WonderPantanal, FieldLevel::WonderVolcanic};

std::vector<CityStateEnum> stateCityField = {
    CityStateEnum::carthage,
    CityStateEnum::kaboul,
    CityStateEnum::mohenjoDaro,
    CityStateEnum::kumasi,
    CityStateEnum::seoul,
    CityStateEnum::geneva,
    CityStateEnum::buenosAires,
    CityStateEnum::bruxelles};

/*!
 * @brief Map constructor
 * @param width Width of the map to instantiate
 * @param height Height of the map to instantiate
 */
Map::Map(unsigned width, unsigned height)
{
    this->height = height;
    this->width = width;
}

void Map::init()
{
    if (isInizialize)
    {
        return;
    }
    for (unsigned i = 0; i < this->height; i++)
    {
        for (unsigned j = 0; j < this->width; j++)
        {
            mapOfTheGame.push_back(std::make_shared<Hexagon>());
        }
    }
    isInizialize = true;
}

void Map::setMapHeight(unsigned height)
{
    this->height = height;
}

void Map::setMapWidth(unsigned width)
{
    this->width = width;
}

unsigned Map::getMapHeight()
{
    return this->height;
}

unsigned Map::getMapWidth()
{
    return this->width;
}

bool wonderCondition(std::shared_ptr<Hexagon> hex, std::vector<FieldLevel> &remWonder)
{
    return hex->getFieldLevel() != FieldLevel::Water &&
           remWonder.empty() == false &&
           hex->hexResource == nullptr &&
           rand() % 1000 < 5;
}

/*!
 * @brief Function to generate a random map based on Perlin Noise
 * @param seed Seed to use for the random generation
 */
void Map::generateRandomMap(int seed)
{

    if (!isInizialize)
    {
        init();
    }

    PerlinNoise pn(seed);
    srand(time(NULL));

    std::vector<FieldLevel> remainingWonders(wonderList, wonderList + sizeof(wonderList) / sizeof(wonderList[0]));
    std::vector<bool> remainingCity(stateCityField.size(), true);

    for (unsigned i = 0; i < this->height; i++)
    {
        for (unsigned j = 0; j < this->width; j++)
        {
            double x = (double)j / ((double)this->width);
            double y = (double)i / ((double)this->height);

            double n = pn.noise(PERLIN_NOISE_WIDTH * x, PERLIN_NOISE_WIDTH * y, PERLIN_NOISE_PARAM);
            int field = (int)round(n * (NUMBER_OF_FIELDS + 1));

            std::array<unsigned, 2> position = {i, j};

            switch (field)
            {
            case 0:
                setWater(i, j);
                break;
            case 1:
                mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Grassland);
                addResource(i, j, 2, ResourceEnum::stone);
                addResource(i, j, 2, ResourceEnum::antiquity);
                break;
            case 2:
                mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Hill);
                addResource(i, j, 2, ResourceEnum::stone);
                addResource(i, j, 2, ResourceEnum::antiquity);
                break;
            case 3:
                mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Forest);
                addResource(i, j, 2, ResourceEnum::stone);
                addResource(i, j, 2, ResourceEnum::antiquity);
                break;
            case 4:
                mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Desert);
                addResource(i, j, 12, ResourceEnum::oil);
                addResource(i, j, 6, ResourceEnum::antiquity);
                break;
            case 5:
                mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Mountain);
                addResource(i, j, 12, ResourceEnum::diamond);
                addResource(i, j, 6, ResourceEnum::stone);
                break;
            default:
                mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Water);
                break;
            }

            if (i == 0 || i == this->height - 1 || j == 0 || j == this->width - 1)
            {
                setWater(i, j);
            }
            // 50% chance to be water
            else if (rand() % 10 < 5 && (i == 1 || i == this->height - 2 || j == 1 || j == this->width - 2))
            {
                setWater(i, j);
            }
            // 10% chance to be water
            else if (rand() % 10 < 1 && (i == 2 || i == this->height - 3 || j == 2 || j == this->width - 3))
            {
                setWater(i, j);
            }

            if (wonderCondition(mapOfTheGame[i * this->width + j], remainingWonders))
            {
                int wonderIndex = rand() % remainingWonders.size(); // remainingWonders is not empty here
                FieldLevel wonder = remainingWonders[wonderIndex];
                remainingWonders.erase(remainingWonders.begin() + wonderIndex);
                mapOfTheGame[i * this->width + j]->setFieldType(wonder);
            }

            if (mapOfTheGame[i * this->width + j]->hexResource != nullptr || mapOfTheGame[i * this->width + j]->getFieldLevel() == FieldLevel::Water)
            {
                continue;
            }

            if (rand() % 100 < 5)
            {
                std::shared_ptr<shared::BarbarianVillage> barbareVillage = std::make_shared<shared::BarbarianVillage>();
                std::shared_ptr<shared::Barbarian> barbare = std::make_shared<shared::Barbarian>();
                mapOfTheGame[i * this->width + j]->addElement(std::make_shared<variantElement>(*barbare));
                mapOfTheGame[i * this->width + j]->addElement(std::make_shared<variantElement>(*barbareVillage));
                continue;
            }

            if (rand() % 100 < 3)
            {
                std::vector<int> remainingIndex;
                for (unsigned k = 0; k < stateCityField.size(); k++)
                {
                    if (remainingCity[k])
                    {
                        remainingIndex.push_back(k);
                    }
                }
                if (remainingIndex.empty() == false)
                {
                    int index = remainingIndex[rand() % remainingIndex.size()]; // remainingIndex is not empty
                    std::shared_ptr<shared::City> city = std::make_shared<shared::City>(position);
                    city->setStateCity(stateCityField[index]);
                    remainingCity[index] = false;
                    mapOfTheGame[i * this->width + j]->addElement(std::make_shared<variantElement>(*city));
                }
            }
        }
    }
}

void Map::addResource(int x, int y, int procent, ResourceEnum resource)
{
    if (rand() % 100 < procent)
    {
        mapOfTheGame[x * this->width + y]->hexResource = std::make_shared<Resource>(resource);
    }
}

void Map::setWater(int x, int y)
{
    mapOfTheGame[x * this->width + y]->setFieldType(FieldLevel::Water);
    mapOfTheGame[x * this->width + y]->clearElement();
    mapOfTheGame[x * this->width + y]->hexResource = nullptr;
}

/*!
 * @brief Operator to access a specific hexagon of the map
 * @param x X coordinate of the hexagon
 * @param y Y coordinate of the hexagon
 * @return Pointer to the hexagon
 */
std::shared_ptr<Hexagon> Map::operator()(unsigned x, unsigned y)
{
    if (x < this->width && y < this->height && mapOfTheGame.size() > 0)
    {
        return mapOfTheGame[y * this->width + x];
    }
    else
    {
        return nullptr;
    }
}

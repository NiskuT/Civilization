#include <shared.hpp>
#include <math.h>

namespace shared
{

    Map::Map(unsigned length, unsigned height)
    {
        this->height = height;
        this->width = length;
        for (unsigned i = 0; i < height; i++)
        {
            for (unsigned j = 0; j < length; j++)
            {
                mapOfTheGame.push_back(new Hexagon());
            }
        }
    }

    unsigned Map::getMapHeight()
    {
        return this->height;
    }

    unsigned Map::getMapWidth()
    {
        return this->width;
    }

    void Map::generateRandomMap(int seed)
    {
        PerlinNoise pn(seed);

        for (unsigned i = 0; i < this->height; i++)
        {
            for (unsigned j = 0; j < this->width; j++)
            {
                double x = (double)j / ((double)this->width);
                double y = (double)i / ((double)this->height);

                double n = pn.noise(10 * x, 10 * y, 0.8);
                int field = (int) round(n * 5);

                switch(field)
                {
                    case 0:
                        mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Water);
                        break;
                    case 1:
                        mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Grassland);
                        break;
                    case 2:
                        mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Hill);
                        break;
                    case 3:
                        mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Forest);
                        break;
                    case 4:
                        mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Desert);
                        break;
                    case 5:
                        mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Mountain);
                        break;
                    default:
                        mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Water);
                        break;
                }

                if (i == 0 || i == this->height - 1 || j == 0 || j == this->width - 1)
                {
                    mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Water);
                } else if (i == 1 || i == this->height - 2 || j == 1 || j == this->width - 2)
                {
                    // 70% chance to be water
                    if (rand() % 10 < 7) mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Water);
                }
                else if (i == 2 || i == this->height - 3 || j == 2 || j == this->width - 3)
                {
                    // 30% chance to be water
                    if (rand() % 10 < 3) mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::Water);
                }
                else {
                    // 5% chance to be a random wonder
                    if (rand() % 100 < 1) mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::WonderEverest);
                    else if (rand() % 100 < 1) mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::WonderGalapagos);
                    else if (rand() % 100 < 1) mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::WonderKilimanjaro);
                    else if (rand() % 100 < 1) mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::WonderMessa);
                    else if (rand() % 100 < 1) mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::WonderPantanal);
                    else if (rand() % 100 < 1) mapOfTheGame[i * this->width + j]->setFieldType(FieldLevel::WonderVolcanic);
                }
            }
        }

    }

    Hexagon *Map::operator()(unsigned x, unsigned y)
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

    Map::~Map()
    {
        for (unsigned i = 0; i < mapOfTheGame.size(); i++)
        {
            delete mapOfTheGame[i];
        }
    }

}

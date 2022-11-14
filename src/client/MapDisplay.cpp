#include <client.hpp>
#include <iostream>
#include <fstream>
#include <json/json.h>

namespace client
{

    MapDisplay::MapDisplay()
    {

        hexClientDisplay.loadHexTexture("../ressources/img/map/field.png", sf::Vector2u(83, 85), 15, 11);

        Json::Value root;
        std::ifstream json_file("../ressources/img/map/files.json", std::ifstream::binary);
        json_file >> root;

        for (unsigned index = 0; index < root["data"].size(); ++index) {
            temporaryElementData.emplace_back(
                root["data"][index]["i"].asInt(),
                root["data"][index]["j"].asInt(),
                root["data"][index]["k"].asInt(),
                root["data"][index]["l"].asInt(),
                root["data"][index]["path"].asString());
        }

        for (TextureData &t : temporaryElementData) {
            t.loadTexture();
            elementSprites.emplace_back(t.texture);
            elementSprites.back().move(sf::Vector2f(t.position.at(0) * 1.f, t.position.at(1) * 1.f));
        };
    }

    void MapDisplay::setOffset(int xOffset, int yOffset)
    {

        for (sf::Sprite sprite : elementSprites)
        {
            sprite.move(sf::Vector2f(xOffset * 1.f, yOffset * 1.f));
        }
        hexClientDisplay.updateHexPosition(sf::Vector2u(83, 85), 15, 11);
    }

}
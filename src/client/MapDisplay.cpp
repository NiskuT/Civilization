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
                root["data"][index]["x"].asInt(),
                root["data"][index]["y"].asInt(),
                root["data"][index]["xOffset"].asInt(),
                root["data"][index]["yOffset"].asInt(),
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
        /*
        for(unsigned i = 0; i < elementSprites.size(); i++){
            elementSprites.at(i).move((xOffset + temporaryElementData.at(i).position.at(0)), (yOffset + temporaryElementData.at(i).position.at(1)));
        }*/
        hexClientDisplay.updateHexPosition(sf::Vector2u(83, 85), 15, 11, xOffset, yOffset);

        for (sf::Sprite sprite : elementSprites)
        {
            sprite.move(sf::Vector2f(xOffset * 1.f, yOffset * 1.f));
        }
    }

}
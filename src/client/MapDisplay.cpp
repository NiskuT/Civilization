#include <client.hpp>
#include <iostream>

namespace client {

MapDisplay::MapDisplay() {
    
    hexClientDisplay.loadHexTexture("../ressources/img/map/field.png", sf::Vector2u(83, 85), 15, 11);

    temporaryElementData.emplace_back(0,4,20,21,"../ressources/img/map/ressource-oil.png");
    temporaryElementData.emplace_back(2,6,20,21,"../ressources/img/map/ressource-stone.png");
    temporaryElementData.emplace_back(6,7,20,21,"../ressources/img/map/ressource-antic.png");
    temporaryElementData.emplace_back(7,11,20,21,"../ressources/img/map/ressource-stone.png");
    temporaryElementData.emplace_back(10,6,20,21,"../ressources/img/map/ressource-diamond.png");
    temporaryElementData.emplace_back(1,8,20,22,"../ressources/img/map/player-2.png");
    temporaryElementData.emplace_back(2,9,20,22,"../ressources/img/map/player-2.png");
    temporaryElementData.emplace_back(2,10,20,22,"../ressources/img/map/player-2.png");
    temporaryElementData.emplace_back(4,10,20,22,"../ressources/img/map/player-2.png");
    temporaryElementData.emplace_back(5,11,20,22,"../ressources/img/map/player-2.png");
    temporaryElementData.emplace_back(1,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    temporaryElementData.emplace_back(3,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    temporaryElementData.emplace_back(3,8,20,22,"../ressources/img/map/player-2-reinforced.png");
    temporaryElementData.emplace_back(5,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    temporaryElementData.emplace_back(6,10,20,22,"../ressources/img/map/player-2-reinforced.png");
    temporaryElementData.emplace_back(2,8,10,9,"../ressources/img/map/player-2-mature.png");
    temporaryElementData.emplace_back(3,10,10,9,"../ressources/img/map/player-2-capital.png");
    temporaryElementData.emplace_back(4,8,10,9,"../ressources/img/map/player-2-city.png");
    temporaryElementData.emplace_back(5,10,10,9,"../ressources/img/map/player-2-city.png");
    temporaryElementData.emplace_back(8,11,17,17,"../ressources/img/map/player-2-caravan.png");
    temporaryElementData.emplace_back(5,7,20,22,"../ressources/img/map/player-1.png");
    temporaryElementData.emplace_back(6,5,20,22,"../ressources/img/map/player-1.png");
    temporaryElementData.emplace_back(6,2,20,22,"../ressources/img/map/player-1.png");
    temporaryElementData.emplace_back(7,3,20,22,"../ressources/img/map/player-1.png");
    temporaryElementData.emplace_back(7,7,20,22,"../ressources/img/map/player-1.png");
    temporaryElementData.emplace_back(6,6,20,22,"../ressources/img/map/player-1-reinforced.png");
    temporaryElementData.emplace_back(4,5,20,22,"../ressources/img/map/player-1-reinforced.png");
    temporaryElementData.emplace_back(6,4,20,22,"../ressources/img/map/player-1-reinforced.png");
    temporaryElementData.emplace_back(8,6,20,22,"../ressources/img/map/player-1-reinforced.png");
    temporaryElementData.emplace_back(5,6,10,9,"../ressources/img/map/player-1-capital.png");
    temporaryElementData.emplace_back(7,6,10,9,"../ressources/img/map/player-1-mature.png");
    temporaryElementData.emplace_back(6,3,10,9,"../ressources/img/map/player-1-city.png");
    temporaryElementData.emplace_back(7,9,17,17,"../ressources/img/map/player-1-caravan.png");
    temporaryElementData.emplace_back(7,10,10,9,"../ressources/img/map/state-city-science.png");
    temporaryElementData.emplace_back(9,12,10,9,"../ressources/img/map/state-city-culture.png");
    temporaryElementData.emplace_back(9,4,10,9,"../ressources/img/map/state-city-army.png");
    temporaryElementData.emplace_back(0,3,20,22,"../ressources/img/map/barbarians.png");
    temporaryElementData.emplace_back(1,6,20,22,"../ressources/img/map/barbarians.png");
    temporaryElementData.emplace_back(6,6,20,22,"../ressources/img/map/barbarians.png");
    temporaryElementData.emplace_back(8,4,20,22,"../ressources/img/map/barbarians.png");
    temporaryElementData.emplace_back(0,7,20,22,"../ressources/img/map/barbarian-hutte.png");
    temporaryElementData.emplace_back(2,3,20,22,"../ressources/img/map/barbarian-hutte.png");
    temporaryElementData.emplace_back(9,6,20,22,"../ressources/img/map/barbarian-hutte.png");

    for(TextureData& t : temporaryElementData) {
        t.loadTexture();
        elementSprites.emplace_back(t.texture);
        elementSprites.back().move(sf::Vector2f(t.position.at(0) * 1.f, t.position.at(1) * 1.f));
    };
}

void MapDisplay::setOffset (int xOffset, int yOffset){

    for(sf::Sprite sprite : elementSprites)
    {
        sprite.move(sf::Vector2f(xOffset * 1.f, yOffset * 1.f));
    }
    hexClientDisplay.updateHexPosition(sf::Vector2u(83, 85), 15, 11);

}

}
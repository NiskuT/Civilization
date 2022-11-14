#include <client.hpp>
#include <iostream>

namespace client {

MapDisplay::MapDisplay() {

    int level[165] =
    {
        0, 0, 5, 4, 4, 2, 5, 2, 1, 2, 5, 0, 0, 0, 0,
        0, 0, 0, 5, 0, 1, 5, 5, 5, 5, 1, 1, 0, 0, 0,
        0, 0, 0, 3, 0, 1, 5, 0, 1, 1, 3, 4, 0, 0, 0,
        0, 0, 0, 0, 1, 2, 5, 3, 2, 3, 1, 4, 0, 0, 0,
        0, 0, 0, 0, 4, 5, 5, 2, 3, 0, 3, 2, 0, 0, 0,
        0, 0, 0, 0, 2, 5, 1, 3, 3, 2, 2, 5, 0, 0, 0,
        0, 0, 5, 1, 2, 1, 5, 3, 1, 5, 2, 5, 4, 0, 0,
        0, 0, 0, 1, 2, 0, 4, 2, 5, 3, 1, 4, 5, 0, 0,
        0, 0, 0, 0, 2, 0, 5, 4, 2, 0, 0, 3, 2, 2, 0,
        0, 0, 0, 0, 1, 1, 5, 3, 0, 5, 2, 4, 2, 2, 0,
        0, 0, 0, 0, 0, 1, 5, 5, 0, 1, 0, 0, 4, 2, 0,
    };
    
    hexagon.load("../ressources/img/map/field.png", sf::Vector2u(83, 85), level, 15, 11);

    elementData.emplace_back(0,4,20,21,"../ressources/img/map/ressource-oil.png");
    elementData.emplace_back(2,6,20,21,"../ressources/img/map/ressource-stone.png");
    elementData.emplace_back(6,7,20,21,"../ressources/img/map/ressource-antic.png");
    elementData.emplace_back(7,11,20,21,"../ressources/img/map/ressource-stone.png");
    elementData.emplace_back(10,6,20,21,"../ressources/img/map/ressource-diamond.png");
    elementData.emplace_back(1,8,20,22,"../ressources/img/map/player-2.png");
    elementData.emplace_back(2,9,20,22,"../ressources/img/map/player-2.png");
    elementData.emplace_back(2,10,20,22,"../ressources/img/map/player-2.png");
    elementData.emplace_back(4,10,20,22,"../ressources/img/map/player-2.png");
    elementData.emplace_back(5,11,20,22,"../ressources/img/map/player-2.png");
    elementData.emplace_back(1,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData.emplace_back(3,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData.emplace_back(3,8,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData.emplace_back(5,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData.emplace_back(6,10,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData.emplace_back(2,8,10,9,"../ressources/img/map/player-2-mature.png");
    elementData.emplace_back(3,10,10,9,"../ressources/img/map/player-2-capital.png");
    elementData.emplace_back(4,8,10,9,"../ressources/img/map/player-2-city.png");
    elementData.emplace_back(5,10,10,9,"../ressources/img/map/player-2-city.png");
    elementData.emplace_back(8,11,17,17,"../ressources/img/map/player-2-caravan.png");
    elementData.emplace_back(5,7,20,22,"../ressources/img/map/player-1.png");
    elementData.emplace_back(6,5,20,22,"../ressources/img/map/player-1.png");
    elementData.emplace_back(6,2,20,22,"../ressources/img/map/player-1.png");
    elementData.emplace_back(7,3,20,22,"../ressources/img/map/player-1.png");
    elementData.emplace_back(7,7,20,22,"../ressources/img/map/player-1.png");
    elementData.emplace_back(6,6,20,22,"../ressources/img/map/player-1-reinforced.png");
    elementData.emplace_back(4,5,20,22,"../ressources/img/map/player-1-reinforced.png");
    elementData.emplace_back(6,4,20,22,"../ressources/img/map/player-1-reinforced.png");
    elementData.emplace_back(8,6,20,22,"../ressources/img/map/player-1-reinforced.png");
    elementData.emplace_back(5,6,10,9,"../ressources/img/map/player-1-capital.png");
    elementData.emplace_back(7,6,10,9,"../ressources/img/map/player-1-mature.png");
    elementData.emplace_back(6,3,10,9,"../ressources/img/map/player-1-city.png");
    elementData.emplace_back(7,9,17,17,"../ressources/img/map/player-1-caravan.png");
    elementData.emplace_back(7,10,10,9,"../ressources/img/map/state-city-science.png");
    elementData.emplace_back(9,12,10,9,"../ressources/img/map/state-city-culture.png");
    elementData.emplace_back(9,4,10,9,"../ressources/img/map/state-city-army.png");
    elementData.emplace_back(0,3,20,22,"../ressources/img/map/barbarians.png");
    elementData.emplace_back(1,6,20,22,"../ressources/img/map/barbarians.png");
    elementData.emplace_back(6,6,20,22,"../ressources/img/map/barbarians.png");
    elementData.emplace_back(8,4,20,22,"../ressources/img/map/barbarians.png");
    elementData.emplace_back(0,7,20,22,"../ressources/img/map/barbarian-hutte.png");
    elementData.emplace_back(2,3,20,22,"../ressources/img/map/barbarian-hutte.png");
    elementData.emplace_back(9,6,20,22,"../ressources/img/map/barbarian-hutte.png");

    for(TextureData& t : elementData) {
        t.loadTexture();
        elementSprites.emplace_back(t.texture);
        elementSprites.back().move(sf::Vector2f(t.position.at(0) * 1.f, t.position.at(1) * 1.f));
    };
}

void MapDisplay::setOffset (int xOffset, int yOffset){

    int level[165] =
    {
        0, 0, 5, 4, 4, 2, 5, 2, 1, 2, 5, 0, 0, 0, 0,
        0, 0, 0, 5, 0, 1, 5, 5, 5, 5, 1, 1, 0, 0, 0,
        0, 0, 0, 3, 0, 1, 5, 0, 1, 1, 3, 4, 0, 0, 0,
        0, 0, 0, 0, 1, 2, 5, 3, 2, 3, 1, 4, 0, 0, 0,
        0, 0, 0, 0, 4, 5, 5, 2, 3, 0, 3, 2, 0, 0, 0,
        0, 0, 0, 0, 2, 5, 1, 3, 3, 2, 2, 5, 0, 0, 0,
        0, 0, 5, 1, 2, 1, 5, 3, 1, 5, 2, 5, 4, 0, 0,
        0, 0, 0, 1, 2, 0, 4, 2, 5, 3, 1, 4, 5, 0, 0,
        0, 0, 0, 0, 2, 0, 5, 4, 2, 0, 0, 3, 2, 2, 0,
        0, 0, 0, 0, 1, 1, 5, 3, 0, 5, 2, 4, 2, 2, 0,
        0, 0, 0, 0, 0, 1, 5, 5, 0, 1, 0, 0, 4, 2, 0,
    };

    for(sf::Sprite sprite : elementSprites)
    {
        sprite.move(sf::Vector2f(xOffset * 1.f, yOffset * 1.f));
    }
    hexagon.update(sf::Vector2u(83, 85), level, 15, 11);

}

}
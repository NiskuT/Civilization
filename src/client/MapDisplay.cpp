#include <client.hpp>
#include <iostream>

#define Y_OFFSET -21

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

    elementData.push_back(update(0,4,20,21,"../ressources/img/map/ressource-oil.png"));
    elementData.push_back(update(2,6,20,21,"../ressources/img/map/ressource-stone.png"));
    elementData.push_back(update(6,7,20,21,"../ressources/img/map/ressource-antic.png"));
    elementData.push_back(update(7,11,20,21,"../ressources/img/map/ressource-stone.png"));
    elementData.push_back(update(10,6,20,21,"../ressources/img/map/ressource-diamond.png"));
    elementData.push_back(update(1,8,20,22,"../ressources/img/map/player-2.png"));
    elementData.push_back(update(2,9,20,22,"../ressources/img/map/player-2.png"));
    elementData.push_back(update(2,10,20,22,"../ressources/img/map/player-2.png"));
    elementData.push_back(update(4,10,20,22,"../ressources/img/map/player-2.png"));
    elementData.push_back(update(5,11,20,22,"../ressources/img/map/player-2.png"));
    elementData.push_back(update(1,9,20,22,"../ressources/img/map/player-2-reinforced.png"));
    elementData.push_back(update(3,9,20,22,"../ressources/img/map/player-2-reinforced.png"));
    elementData.push_back(update(3,8,20,22,"../ressources/img/map/player-2-reinforced.png"));
    elementData.push_back(update(5,9,20,22,"../ressources/img/map/player-2-reinforced.png"));
    elementData.push_back(update(6,10,20,22,"../ressources/img/map/player-2-reinforced.png"));
    elementData.push_back(update(2,8,10,9,"../ressources/img/map/player-2-mature.png"));
    elementData.push_back(update(3,10,10,9,"../ressources/img/map/player-2-capital.png"));
    elementData.push_back(update(4,8,10,9,"../ressources/img/map/player-2-city.png"));
    elementData.push_back(update(5,10,10,9,"../ressources/img/map/player-2-city.png"));
    elementData.push_back(update(8,11,17,17,"../ressources/img/map/player-2-caravan.png"));
    elementData.push_back(update(5,7,20,22,"../ressources/img/map/player-1.png"));
    elementData.push_back(update(6,5,20,22,"../ressources/img/map/player-1.png"));
    elementData.push_back(update(6,2,20,22,"../ressources/img/map/player-1.png"));
    elementData.push_back(update(7,3,20,22,"../ressources/img/map/player-1.png"));
    elementData.push_back(update(7,7,20,22,"../ressources/img/map/player-1.png"));
    elementData.push_back(update(6,6,20,22,"../ressources/img/map/player-1-reinforced.png"));
    elementData.push_back(update(4,5,20,22,"../ressources/img/map/player-1-reinforced.png"));
    elementData.push_back(update(6,4,20,22,"../ressources/img/map/player-1-reinforced.png"));
    elementData.push_back(update(8,6,20,22,"../ressources/img/map/player-1-reinforced.png"));
    elementData.push_back(update(5,6,10,9,"../ressources/img/map/player-1-capital.png"));
    elementData.push_back(update(7,6,10,9,"../ressources/img/map/player-1-mature.png"));
    elementData.push_back(update(6,3,10,9,"../ressources/img/map/player-1-city.png"));
    elementData.push_back(update(7,9,17,17,"../ressources/img/map/player-1-caravan.png"));
    elementData.push_back(update(7,10,10,9,"../ressources/img/map/state-city-science.png"));
    elementData.push_back(update(9,12,10,9,"../ressources/img/map/state-city-culture.png"));
    elementData.push_back(update(9,4,10,9,"../ressources/img/map/state-city-army.png"));
    elementData.push_back(update(0,3,20,22,"../ressources/img/map/barbarians.png"));
    elementData.push_back(update(1,6,20,22,"../ressources/img/map/barbarians.png"));
    elementData.push_back(update(6,6,20,22,"../ressources/img/map/barbarians.png"));
    elementData.push_back(update(8,4,20,22,"../ressources/img/map/barbarians.png"));
    elementData.push_back(update(0,7,20,22,"../ressources/img/map/barbarian-hutte.png"));
    elementData.push_back(update(2,3,20,22,"../ressources/img/map/barbarian-hutte.png"));
    elementData.push_back(update(9,6,20,22,"../ressources/img/map/barbarian-hutte.png"));

    for(int i =0; i < elementData.size(); i++)
    {
        ElementDisplay newElement;
        elements.push_back(newElement);
        elements.at(i).update(elementData.at(i));

        Sprite newSprite;
        elementSprites.push_back(newSprite);
        elementSprites.at(i).setTexture(elements.at(i).texture);
        elementSprites.at(i).move(sf::Vector2f(elements.at(i).position.at(1) * 1.f, elements.at(i).position.at(1) * 1.f));
    };
}

void MapDisplay::setOffset (int xOffset, int yOffset){

    for(int i =0; i < elementData.size(); i++)
    {
        elementSprites.at(1).move(sf::Vector2f(xOffset * 1.f, yOffset * 1.f));
    }
    hexagon.update(sf::Vector2u(83, 85), level, 15, 11);

}

}
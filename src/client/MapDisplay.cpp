#include <client.hpp>
#include <iostream>

#define LEN_ELEMENT 43

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

    for(int i = 0; i < LEN_ELEMENT; i ++){
        TextureData newTextureData;
        elementData.push_back(newTextureData);
    }

    elementData[0].loadData(0,4,20,21,"../ressources/img/map/ressource-oil.png");
    elementData[1].loadData(2,6,20,21,"../ressources/img/map/ressource-stone.png");
    elementData[2].loadData(6,7,20,21,"../ressources/img/map/ressource-antic.png");
    elementData[3].loadData(7,11,20,21,"../ressources/img/map/ressource-stone.png");
    elementData[4].loadData(10,6,20,21,"../ressources/img/map/ressource-diamond.png");
    elementData[5].loadData(1,8,20,22,"../ressources/img/map/player-2.png");
    elementData[6].loadData(2,9,20,22,"../ressources/img/map/player-2.png");
    elementData[7].loadData(2,10,20,22,"../ressources/img/map/player-2.png");
    elementData[8].loadData(4,10,20,22,"../ressources/img/map/player-2.png");
    elementData[9].loadData(5,11,20,22,"../ressources/img/map/player-2.png");
    elementData[10].loadData(1,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData[11].loadData(3,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData[12].loadData(3,8,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData[13].loadData(5,9,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData[14].loadData(6,10,20,22,"../ressources/img/map/player-2-reinforced.png");
    elementData[15].loadData(2,8,10,9,"../ressources/img/map/player-2-mature.png");
    elementData[16].loadData(3,10,10,9,"../ressources/img/map/player-2-capital.png");
    elementData[17].loadData(4,8,10,9,"../ressources/img/map/player-2-city.png");
    elementData[18].loadData(5,10,10,9,"../ressources/img/map/player-2-city.png");
    elementData[19].loadData(8,11,17,17,"../ressources/img/map/player-2-caravan.png");
    elementData[20].loadData(5,7,20,22,"../ressources/img/map/player-1.png");
    elementData[21].loadData(6,5,20,22,"../ressources/img/map/player-1.png");
    elementData[22].loadData(6,2,20,22,"../ressources/img/map/player-1.png");
    elementData[23].loadData(7,3,20,22,"../ressources/img/map/player-1.png");
    elementData[24].loadData(7,7,20,22,"../ressources/img/map/player-1.png");
    elementData[25].loadData(6,6,20,22,"../ressources/img/map/player-1-reinforced.png");
    elementData[26].loadData(4,5,20,22,"../ressources/img/map/player-1-reinforced.png");
    elementData[27].loadData(6,4,20,22,"../ressources/img/map/player-1-reinforced.png");
    elementData[28].loadData(8,6,20,22,"../ressources/img/map/player-1-reinforced.png");
    elementData[29].loadData(5,6,10,9,"../ressources/img/map/player-1-capital.png");
    elementData[30].loadData(7,6,10,9,"../ressources/img/map/player-1-mature.png");
    elementData[31].loadData(6,3,10,9,"../ressources/img/map/player-1-city.png");
    elementData[32].loadData(7,9,17,17,"../ressources/img/map/player-1-caravan.png");
    elementData[33].loadData(7,10,10,9,"../ressources/img/map/state-city-science.png");
    elementData[34].loadData(9,12,10,9,"../ressources/img/map/state-city-culture.png");
    elementData[35].loadData(9,4,10,9,"../ressources/img/map/state-city-army.png");
    elementData[36].loadData(0,3,20,22,"../ressources/img/map/barbarians.png");
    elementData[37].loadData(1,6,20,22,"../ressources/img/map/barbarians.png");
    elementData[38].loadData(6,6,20,22,"../ressources/img/map/barbarians.png");
    elementData[39].loadData(8,4,20,22,"../ressources/img/map/barbarians.png");
    elementData[40].loadData(0,7,20,22,"../ressources/img/map/barbarian-hutte.png");
    elementData[41].loadData(2,3,20,22,"../ressources/img/map/barbarian-hutte.png");
    elementData[42].loadData(9,6,20,22,"../ressources/img/map/barbarian-hutte.png");

    for(int i =0; i < elementData.size(); i++)
    {
        ElementDisplay newElement;
        elements.push_back(newElement);
        elements.at(i).loadTexture(elementData.at(i));

        sf::Sprite newSprite;
        elementSprites.push_back(newSprite);
        elementSprites.at(i).setTexture(elements.at(i).texture);
        elementSprites.at(i).move(sf::Vector2f(elements.at(i).position.at(1) * 1.f, elements.at(i).position.at(1) * 1.f));
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

    for(int i =0; i < elementData.size(); i++)
    {
        elementSprites.at(1).move(sf::Vector2f(xOffset * 1.f, yOffset * 1.f));
    }
    hexagon.update(sf::Vector2u(83, 85), level, 15, 11);

}

}
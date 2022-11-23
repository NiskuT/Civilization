#include <client.hpp>
#include <iostream>
#include <fstream>
#include <json/json.h>

#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0

namespace client {

/*!
* \brief Constructeur
*
* Constructor of GameWindow class
*/
GameWindow::GameWindow() {
    clientGameWindow.create(sf::VideoMode(1231, 725),"Civilization VII");
}

/*!
* \brief Display all the different variable in the screen
*/
void GameWindow::displayWindow() {

    clientGameWindow.clear(sf::Color::Blue);

    for(TextureDisplayer texture : textureToDisplay){

        for(unsigned j = 0; j < texture.getSize(); j++ ){

            clientGameWindow.draw(*texture.getSprite(j));

        }
    }

    clientGameWindow.display();
}

/*!
* \brief Loop that look for events to happend and call displayWindow()
*/
void GameWindow::clientWindow() {

    int turn = 0;

    while (clientGameWindow.isOpen())
    {
        // handle events
        sf::Event event;
        while (clientGameWindow.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                clientGameWindow.close();
        }

        // draw the map
        if (turn == 0) {
            loadTexture();
            turn += 1;
        }
        displayWindow();
    }
}

void GameWindow::loadTexture() {

    std::array<int, 165> level =
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
        0, 0, 0, 0, 0, 1, 5, 5, 0, 1, 0, 0, 4, 2, 0
    };

    std::string hexagonImgPath = "../ressources/img/map/field-";
    std::array<std::string, 12> mapField =   {"water", "grassland", "hill", "forest", "desert", "mountain",
                                            "wonder-everest", "wonder-galapagos", "wonder-kilimanjaro",
                                            "wonder-messa", "wonder-pantanal", "wonder-volcanic"
                                            };

    for(unsigned i {0}; i < mapField.size(); i++){

        std::string mapElementPath = hexagonImgPath + mapField.at(i) + ".png";
        textureToDisplay.emplace_back(mapElementPath);        
    }

    for(int i = 0; i < 165; i++){

        int indexSprite = textureToDisplay.at(level[i]).getSize();

        textureToDisplay.at(level[i]).addMapSprite();

        //textureToDisplay.at(mapTexture.at(mapShared(i%15,(int)(i/15))->getFieldLevel())).addMapSprite();

        textureToDisplay.at(level[i]).setMapSpritePosition(indexSprite, i);

    }

    std::array<int, 2> hexSize = {textureToDisplay.at(0).getWidth(), textureToDisplay.at(0).getHeight()};
    
    Json::Value root;
    std::ifstream json_file("../ressources/img/map/files.json", std::ifstream::binary);
    json_file >> root;

    for (unsigned index = 0; index < root["data"].size(); ++index) {

        textureToDisplay.emplace_back(root["data"][index]["path"].asString());

        textureToDisplay.back().addMapSprite();

        int rank = root["data"][index]["x"].asInt()*15 + root["data"][index]["y"].asInt();

        textureToDisplay.back().setElementSpritePosition(hexSize, 0, rank); 
    }

}

void GameWindow::setMapOffset(int xOffset, int yOffset) {

    for(TextureDisplayer texture : textureToDisplay){

        for(unsigned j = 0; j < texture.getSize(); j++ ){

            //texture.at(j).mooveMapSpritePosition(xOffset, yOffset);

        }
    }
}

}
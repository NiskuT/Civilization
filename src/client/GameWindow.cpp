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
    clientGameWindow.clear(sf::Color::Red);

    for(ElementDisplayer mapSprite : mapElements)
    {
        clientGameWindow.draw(mapSprite());
    }

    for(ElementDisplayer mapSprite : elementToDisplay)
    {
        clientGameWindow.draw(mapSprite());
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
            loadMap();
            turn += 1;
        }
        displayWindow();
    }
}

void GameWindow::loadMap() {
    std::cout << "UpdateMap \n";

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
        0, 0, 0, 0, 0, 1, 5, 5, 0, 1, 0, 0, 4, 2, 0,
    };

    std::string hexagonImgPath = "../ressources/img/map/field-";
    std::array<std::string, 6> mapField = {"water", "grassland", "hill", "forest", "desert", "mountain"};
    std::array<sf::Texture*, 6> mapTexture;

    for(unsigned i {0}; i < mapField.size(); i++){

        std::string mapElementPath = hexagonImgPath + mapField.at(i) + ".png";
        mapTexture.at(i) = new sf::Texture(); 
        if (!image.loadFromFile(mapElementPath))
        {
            std::cout << "Error loading element picture: " << mapElementPath << "\n";
        };
        mapTexture.at(i)->loadFromImage(image);
    }

    for(unsigned int i = 0; i < mapElements.size(); i++){

        mapElements.at(i).loadTextureToSprite(mapTexture.at(level.at(i)));

        mapElements.at(i).setMapSpritePosition(i);

    }

    std::array<int, 2> hexSize = {mapElements.at(0).getWidth(), mapElements.at(0).getHeight()};
    unsigned startElement = mapTexture.size();

    std::vector<sf::Texture*> elementTexture;

    Json::Value root;
    std::ifstream json_file("../ressources/img/map/files.json", std::ifstream::binary);
    json_file >> root;

    for (unsigned index = 0; index < root["data"].size(); ++index) {
   
        elementTexture.emplace_back(new sf::Texture()); 
        if (!image.loadFromFile(root["data"][index]["path"].asString()))
        {
            std::cout << "Error loading element picture: " << root["data"][index]["path"].asString() << "\n";
        };
        elementTexture.at(index)->loadFromImage(image);

        elementToDisplay.emplace_back();
        elementToDisplay.back().loadTextureToSprite(elementTexture.at(index));

        int rank = root["data"][index]["x"].asInt()*15 + root["data"][index]["y"].asInt();

        elementToDisplay.back().setMapSpritePosition(rank); 
    }


    std::cout << "UpdateMap End\n";
}

}
#include <client.hpp>
#include <iostream>

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
    clientMap.setOffset(MAP_X_OFFSET, MAP_Y_OFFSET);
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

    for(sf::Sprite i : clientMap.elementSprites)
    {
        clientGameWindow.draw(i);
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
        image.loadFromFile(mapElementPath);
        /*if (!mapTexture.at(i)->loadFromFile(mapElementPath))
        {
            std::cout << "Error loading element picture: " << mapElementPath << "\n";
        };*/
        mapTexture.at(i)->loadFromImage(image);
    }

    //sf::Rect hexRect;

    for(unsigned int i = 0; i < mapElements.size(); i++){

        mapElements.at(i).loadTextureToSprite(mapTexture.at(level.at(i)));

        mapElements.at(i).setMapSpritePosition(i);

    }

    std::cout << "----4----\n";

    /*std::ifstream json_file("../ressources/img/map/files.json", std::ifstream::binary);
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
    
    for(unsigned int i = 0; i < mapElements.size(); i++){
        std::string mapElementPath = hexagonImgPath + mapField.at(level.at(i)) + ".png";
        mapElements.at(i).setTexturePath(mapElementPath);
        mapElements.at(i).setMapSpritePosition(i);
    }*/


    std::cout << "UpdateMap End\n";
}

}
#include <client.hpp>
#include <iostream>

#define MAP_X_OFFSET 100
#define MAP_Y_OFFSET 100

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

namespace client {

GameWindow::GameWindow() {
    clientGameWindow.create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH),"Civilization VII");
    clientMap.setOffset(100, 100);
}

void GameWindow::displayWindow() {
    clientGameWindow.clear();
    clientGameWindow.draw(background.getSprite());
    clientGameWindow.draw(ladder.getSprite());
    clientGameWindow.draw(techWheel.getSprite());
    clientGameWindow.draw(barbareWheel.getSprite());
    clientGameWindow.draw(clientMap.hexClientDisplay);
    for(sf::Sprite i : clientMap.elementSprites){
        clientGameWindow.draw(i);
    }
    clientGameWindow.display();
}

void GameWindow::gameWindow() {

    // Display the background
    background.loadHudData("../ressources/img/hud/background.png", 1, "background");
    background.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);

    // Display ladder

    ladder.loadHudData("../ressources/img/hud/ladder.png",1 , "ladder");
    ladder.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);
    

    //Display tech wheel

    techWheel.loadHudData("../ressources/img/hud/tech-wheel.png", 0.3f, "techWheel");
    techWheel.updatePlacement(WINDOW_LENGTH,WINDOW_WIDTH, 0);

    // Display event dial (barbare wheel)

    int posBarbareWheel = 0;
    std::string file = "../ressources/img/hud/barbare-wheel-";
    std::string num = std::to_string(posBarbareWheel);
    std::string format= ".png";
    std::string fileToLoad = file + num + format;
    barbareWheel.loadHudData(fileToLoad, 0.3f, "barbareWheel");
    barbareWheel.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);


    // Display card

/*
    std::vector<std::string> cardType = {"army", "culture", "economy", "industry", "science"};
    std::vector<HudDisplay> priorityCards;
    for (int i=0; i<1; i++) {
        std::string file = "../ressources/img/hud/priority-card-";
        std::string type = cardType[i];
        std::string format= ".png";
        std::string fileToLoad = file + type + format;
        priorityCards.emplace_back(fileToLoad, 1, "priorityCard") ;
        priorityCards.back().updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, i);
    }
*/
   
/*
    // Display the map
    sf::Sprite map;
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../ressources/img/map/exemple.png"))
    {
        std::cout << "Error loading image";
    }
    map.setTexture(mapTexture);
    map.move(100*1.f, 100*1.f);
    map.setScale(0.5f, 0.5f);
*/

    while (clientGameWindow.isOpen()){
        // handle events
        sf::Event event;
        while (clientGameWindow.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                clientGameWindow.close();
        }

        // draw the map
        displayWindow();
        }
}

}

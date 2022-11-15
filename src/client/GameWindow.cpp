#include <client.hpp>
#include <iostream>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

namespace client {

GameWindow::GameWindow() {
    clientGameWindow.create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH),"Civilization VII");
    clientMap.setOffset(MAP_X_OFFSET, MAP_Y_OFFSET);
}

void GameWindow::displayWindow() {
    clientGameWindow.clear();
    clientGameWindow.draw(background.getSprite());
    clientGameWindow.draw(techWheel.getSprite());
    clientGameWindow.draw(barbareWheel.getSprite());
    clientGameWindow.draw(clientMap.hexClientDisplay);
    for(sf::Sprite i : clientMap.elementSprites){
        clientGameWindow.draw(i);
    }
    for (int i = 0; i<5; i++) {
         clientGameWindow.draw(priorityCards.at(i).getSprite());
    }
    clientGameWindow.draw(ladder.getSprite());

 
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
    std::string BarbareWheelFile = "../ressources/img/hud/barbare-wheel-";
    std::string wheelNum = std::to_string(posBarbareWheel);
    std::string format= ".png";
    std::string fileToLoad = BarbareWheelFile + wheelNum + format;
    barbareWheel.loadHudData(fileToLoad, 0.3f, "barbareWheel");
    barbareWheel.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);


    // Display card

    std::vector<std::string> cardType = {"army", "culture", "economy", "industry", "science"};
    std::string priorityCardFile = "../ressources/img/hud/priority-card-";
    for (int i = 0; i<5; i++) {
        std::string type = cardType[i];
        std::string priorityFileToLoad = priorityCardFile + type + format;
        priorityCards.at(i).loadHudData(priorityFileToLoad, 1, "priorityCard");
        priorityCards.at(i).updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, i);
    }


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

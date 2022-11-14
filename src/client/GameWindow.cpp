#include <client.hpp>
#include <iostream>

#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0

namespace client {

GameWindow::GameWindow() {
    clientGameWindow.create(sf::VideoMode(1245, 725),"Civilization VII");
    clientMap.setOffset(MAP_X_OFFSET, MAP_Y_OFFSET);
}

void GameWindow::displayWindow() {
    clientGameWindow.clear();
    clientGameWindow.draw(clientMap.hexClientDisplay);
    for(sf::Sprite i : clientMap.elementSprites)
    {
        clientGameWindow.draw(i);
    }
    clientGameWindow.display();
}

void GameWindow::gameWindow() {

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
        displayWindow();
    }
}

}
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
    clientGameWindow.create(sf::VideoMode(1245, 725),"Civilization VII");
    clientMap.setOffset(MAP_X_OFFSET, MAP_Y_OFFSET);
}

/*!
* \brief Display all the different variable in the screen
*/
void GameWindow::displayWindow() {
    clientGameWindow.clear();
    clientGameWindow.draw(clientMap.hexClientDisplay);
    for(sf::Sprite i : clientMap.elementSprites)
    {
        clientGameWindow.draw(i);
    }
    clientGameWindow.display();
}

/*!
* \brief Loop that look for events to happend and call displayWindow()
*/
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
#include <client.hpp>
#include <iostream>

#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0

namespace client {

GameWindow::GameWindow() {
    window.create(sf::VideoMode(1245, 725),"Civilization VII");
    map.setOffset(MAP_X_OFFSET, MAP_Y_OFFSET);
}

void GameWindow::displayWindow() {
    window.clear();
    window.draw(map.hexagon);
    for(sf::Sprite i : map.elementSprites)
    {
        window.draw(i);
    }
    window.display();
}

void GameWindow::gameWindow() {

    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // draw the map
        displayWindow();
    }
}

}
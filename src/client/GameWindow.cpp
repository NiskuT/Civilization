#include <client.hpp>
#include <iostream>

#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0
#define LEN_ELEMENT 43

namespace client {

GameWindow::GameWindow() {
    window.create(sf::VideoMode(1245, 725),"Civilization VII");
    map.setOffset(MAP_X_OFFSET, MAP_Y_OFFSET);
}

void GameWindow::game() {

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
        display();
    }
}

void GameWindow::display() {
    window.clear();
    window.draw(map.hexagon);
    for(int i =0; i < LEN_ELEMENT; i++)
    {
        window.draw(map.elementSprites[i]);
    }
    window.display();
}

}
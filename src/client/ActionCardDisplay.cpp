#include <client.hpp>
#include <iostream>

namespace client {

    void ActionCardDisplay::loadTitle(std::string title, int x, int y) {
    
        if(!priorityFont.loadFromFile("../ressources/img/hud/font.otf")){
        std::cout << "font not loaded\n" ;
        }

        std::string upperFirstLetter = title;
        upperFirstLetter[0] = toupper(upperFirstLetter[0]);

        titleCard.setFont(priorityFont);
        titleCard.setString(upperFirstLetter);
        titleCard.setCharacterSize(25);
        titleCard.setStyle(sf::Text::Bold);
        titleCard.setColor(sf::Color::Black);
        titleCard.setPosition(x, y);
    }
}
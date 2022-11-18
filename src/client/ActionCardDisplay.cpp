#include <client.hpp>
#include <iostream>

namespace client {

    void PriorityCardDisplay::loadTitle(std::string title, sf::Vector2f position, int xCardSize) {
    
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
        sf::Rect titleScale = titleCard.getLocalBounds();
        int xOffset = (xCardSize - titleScale.width)/2 ;
        titleCard.setPosition(position.x + xOffset, position.y);
    }
}
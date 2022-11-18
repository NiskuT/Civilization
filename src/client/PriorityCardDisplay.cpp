#include <client.hpp>
#include <iostream>
#include <json/json.h>

namespace client {

/*void PriorityCardDisplay::updatePlacement(int length, int width, int priorityCard){

    sf::Vector2<unsigned int> size = this->texture.getSize();
    int offsetLength = 0;
    int offsetWidth = 0;

    offsetLength = 249*priorityCard + 185;
    offsetWidth = width-size.y + 140; 
    this->sprite.move(offsetLength, offsetWidth);
}*/

void PriorityCardDisplay::loadTitle(std::string title, sf::Vector2f position, int xCardSize) {
    
    if(!priorityFont.loadFromFile("../ressources/img/hud/font.otf")){
        std::cout << "font not loaded\n" ;
    }

    std::string upperFirstLetter = title;
    upperFirstLetter[0] = toupper(upperFirstLetter[0]);

    titleCard.setFont(priorityFont);
    titleCard.setString(upperFirstLetter);
    titleCard.setCharacterSize(40);
    titleCard.setStyle(sf::Text::Bold);
    titleCard.setColor(sf::Color::Black);
    sf::Rect titleScale = titleCard.getLocalBounds();
    std::cout << "-------------------\n";
    std::cout << titleScale.width << "\n";
    std::cout << xCardSize << "\n";
    int xOffset = (xCardSize - titleScale.width)/2 ;
    titleCard.setPosition(position.x + xOffset, position.y);
}

}
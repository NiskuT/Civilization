#include <client.hpp>
#include <iostream>

namespace client {

/*void PriorityCardDisplay::updatePlacement(int length, int width, int priorityCard){

    sf::Vector2<unsigned int> size = this->texture.getSize();
    int offsetLength = 0;
    int offsetWidth = 0;

    offsetLength = 249*priorityCard + 185;
    offsetWidth = width-size.y + 140; 
    this->sprite.move(offsetLength, offsetWidth);
}*/

void PriorityCardDisplay::loadTitle(std::string title, int x, int y) {
    
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
    titleCard.setPosition(x, y);
}

}
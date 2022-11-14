#include <client.hpp>
#include <iostream>

namespace client {

void HudDisplay::loadHudData(std::string path, float scale, std::string type ) {
    this->sprite = sprite;
    this->texture = texture;
    this->pathFile = path;
    this->scale = scale;
    this->type = type;

    if (!texture.loadFromFile(pathFile)){
        std::cout << "Error loading background";
    }
    
    sprite.setScale(scale, scale);
    sprite.setTexture(texture);

}

sf::Sprite HudDisplay::getSprite() {return sprite;};

void HudDisplay::updatePlacement(int length, int width, int priorityCard){
sf::Vector2<unsigned int> size = this->texture.getSize();
int offsetLength = 0;
int offsetWidth = 0;
int rotation = 0; 
    if (this->type == "background") {
        
        offsetLength = -((size.x-length)/2);
        offsetWidth = -((size.y-width)/2);
    }

    else if (this->type == "ladder") {
        offsetLength = (length-size.x)/2;
        offsetWidth = (width-size.y);  
    }

    else if (this->type =="techWheel") {
        rotation = 90;
        this->sprite.setOrigin(size.x/2, size.y/2); // set the point of rotation at the center of the image
        offsetLength = length;
        offsetWidth = width; 
    }

    else if (this->type == "barbareWheel") {
        offsetWidth = (width-size.y*0.3);
    }

    else if (this->type == "priorityCard") {
        offsetLength = 10*priorityCard + 185;
        offsetWidth = width-size.y + 140; 
    }

    else {
        std::cout << ("Not good type");
    }
    sprite.rotate(rotation);
    sprite.move(offsetLength, offsetWidth);
    
}

}
#include <client.hpp>
#include <iostream>

namespace client {

void HudDisplay::loadHudData(std::string path, std::string type ) {
    this->pathFile = path;
    this->type = type;

    if (!texture.loadFromFile(pathFile)){
        std::cout << "Error loading " << this->type;
    }
    sprite.setTexture(texture);

}

sf::Sprite HudDisplay::getSprite() {return sprite;};

void HudDisplay::updatePlacement(int length, int width, int priorityCard){
sf::Vector2<unsigned int> size = this->texture.getSize();
int offsetLength = 0;
int offsetWidth = 0;
float scale = 1;
int rotation = 0; 
float ladderProportion = 0.78;
float techWheelProportion = 0.187;
float barbareWheelProportion = 0.0956;
float priorityCardProportion = 0.144;
float actionCardProportion = 0.125;

    if (this->type == "background") {
        offsetLength = -((size.x-length)/2);
        offsetWidth = -((size.y-width)/2);
    }

    else if (this->type == "ladder") {
        scale = float(ladderProportion)/(float(size.x)/float(length));
        offsetLength = (length-size.x*scale)/2;
        offsetWidth = (width-size.y*scale);
    }

    else if (this->type =="techWheel") {
        rotation = 45;
        scale = techWheelProportion/(float(size.x)/float(length));
        this->sprite.setOrigin(size.x/2, size.y/2); // set the point of rotation at the center of the image
        offsetLength = length;
        offsetWidth = width; 
        sprite.rotate(rotation);
    }

    else if (this->type == "barbareWheel") {
        scale = barbareWheelProportion/(float(size.x)/float(length));
        offsetWidth = (width-size.y*scale);
    }

    else if (this->type == "priorityCard") {
        scale = priorityCardProportion/(float(size.x)/float(length));
        float fisrtLengthOffsetScale = float(185) / float(1600); // Offset of the first priority card
        float priorityCardOffset = float(249) / float(1600); // Offset between each card
        float proportionWidth = float(140) / float(900);
        offsetLength = priorityCardOffset*length*priorityCard + fisrtLengthOffsetScale*length; // have to change name "prioritycard" --> it corresponds to the number of the priority card in the ladder
        offsetWidth = width-size.y*scale + proportionWidth*width; 
    }

    else if (this->type == "actionCard") {
        scale = actionCardProportion/(float(size.x)/float(length));
        float rightOffset = float(10) / float(1600);
        float upOffset = float(900/4) / float(900);
        offsetLength = length - size.x*scale - rightOffset*length;
        offsetWidth = upOffset*width + (size.y*scale+10)*priorityCard;
    }

    else {
        std::cout << ("Not good type");
    }
    this->sprite.setScale(scale,scale);
    this->sprite.move(offsetLength, offsetWidth);
}

}
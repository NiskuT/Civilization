#include <client.hpp>
#include <iostream>

namespace client {
    
sf::Texture TextureDisplayer::operator()()
{
   return *this->texture;
}

TextureDisplayer::TextureDisplayer(std::string pathImage)
{    
    this->image = new sf::Image();
    if (!this->image->loadFromFile(pathImage))
    {
        std::cout << "Error loading element picture: " << pathImage << "\n";
    }
    this->texture = new sf::Texture();
    this->texture->loadFromImage(*(this->image));

    int nameStartPosition = 0;
    for(int i = 0; i < pathImage.size(); i++){
        if (pathImage[i] == ('/')){
            nameStartPosition = i;
        }
    }

    this->type = pathImage.substr(nameStartPosition + 1, pathImage.size() - nameStartPosition - 5);
}

TextureDisplayer::~TextureDisplayer()
{    
    //delete this->texture;
}

void TextureDisplayer::addMapSprite()
{
    sf::Sprite* newSprite = new sf::Sprite(*(this->texture));
    this->sprites.push_back(*newSprite);
}

void TextureDisplayer::setMapSpritePosition(int index, int rank)
{
    int xCoordonate = rank % 15;
    int yCoordonate = rank / 15;

    if (yCoordonate%2==0) {
        if (xCoordonate !=14 ){
            this->sprites.at(index).setPosition(sf::Vector2f(41 + xCoordonate * 82, yCoordonate + yCoordonate * 63));
        }
        else{
            this->sprites.at(index).setPosition(sf::Vector2f(-100, -100));
        }
    }
    else {
        this->sprites.at(index).setPosition(sf::Vector2f(xCoordonate * 82, yCoordonate + yCoordonate * 63));
    }
}

void TextureDisplayer::setElementSpritePosition(int index, int rank, std::array<int, 2> hexSize)
{
    int xCoordonate = rank % 15;
    int yCoordonate = rank / 15;

    int xOffset = (hexSize.at(0) - this->sprites.at(index).getLocalBounds().width) / 2;
    int yOffset = (hexSize.at(1) - this->sprites.at(index).getLocalBounds().height) / 2;

    if (yCoordonate%2==0) {
        if (xCoordonate !=14 ){
            this->sprites.at(index).setPosition(sf::Vector2f(xOffset + 41 + xCoordonate * 82, yOffset + yCoordonate + yCoordonate * 63));
        }
        else{
            this->sprites.at(index).setPosition(sf::Vector2f(-100, -100));
        }
    }
    else {
        this->sprites.at(index).setPosition(sf::Vector2f(xOffset + xCoordonate * 82, yOffset +  yCoordonate + yCoordonate * 63));
    }
}


void TextureDisplayer::setHudSpritePosition(float scale, int windowLength, int windowWidth, int rotation)
{
    int xPos = 0;
    int yPos = 0;

    if (this->type == "ladder") {
        xPos = (windowLength-getWidth()*scale)/2;
        yPos = (windowWidth-getHeight()*scale);
    }

    else if (this->type =="tech-wheel") {
        xPos=  windowLength;
        yPos= windowWidth;
        sprites.at(0).setOrigin(getWidth()/2, getHeight()/2);
        sprites.at(0).rotate(rotation);
    }

      else if (this->type == "barbare-wheel-0") {
        yPos = (windowWidth-getWidth()*scale);
        xPos= 0;
    }


    else if (this->type == "priority-card-army") {
        int priorityCardNumber = 0;
        float fisrtLengthOffsetScale = float(185) / float(1600); // Offset of the first priority card
        float priorityCardOffset = float(249) / float(1600); // Offset between each card
        float proportionWidth = float(140) / float(900);
        xPos = priorityCardOffset*windowLength*priorityCardNumber + fisrtLengthOffsetScale*windowLength; // have to change name "prioritycard" --> it corresponds to the number of the priority card in the ladder
        yPos = windowWidth-getHeight()*scale + proportionWidth*windowWidth; 
    }


    else if (this->type == "action-card-army") {
        int actionCardNumber = 0;
        float rightOffset = float(10) / float(1600);
        float upOffset = float(900/4) / float(900);
        xPos = windowLength - getWidth()*scale - rightOffset*windowLength;
        yPos = upOffset*windowWidth + (getHeight()*scale+10)*actionCardNumber;
    }



    this->sprites.at(0).setScale(scale, scale);
    this->sprites.at(0).setPosition(xPos, yPos);
}

unsigned TextureDisplayer::getSize()
{    
    return sprites.size();
}

sf::Sprite* TextureDisplayer::getSprite(unsigned index)
{    
    return &sprites.at((int)(index));
}

int TextureDisplayer::getWidth()
{    
    return getSize() > 0 ? this->sprites.at(0).getLocalBounds().width : 0;
}

int TextureDisplayer::getHeight()
{    
    return getSize() > 0 ? this->sprites.at(0).getLocalBounds().height : 0;
}

}
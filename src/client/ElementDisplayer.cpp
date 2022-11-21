#include <client.hpp>
#include <iostream>

namespace client {
    
sf::Sprite ElementDisplayer::operator()()
{
   return *this->sprite;
}

void ElementDisplayer::loadTextureToSprite(sf::Texture* textureToLoad)
{    
    this->sprite = new sf::Sprite(*textureToLoad);
} 

int ElementDisplayer::getWidth() 
{
    return this->sprite->getLocalBounds().width;
}

int ElementDisplayer::getHeight()
{
    return this->sprite->getLocalBounds().height;
} 

sf::Sprite ElementDisplayer::getSprite()
{    
    return *this->sprite;
} 

void ElementDisplayer::setMapSpritePosition(unsigned int rank)
{
    int xCoordonate = rank % 15;
    int yCoordonate = rank / 15;

    if (yCoordonate%2==0) {
        if (xCoordonate !=14 ){
            this->sprite->setPosition(sf::Vector2f(41 + xCoordonate * 82, yCoordonate + yCoordonate * 63));
        }
        else{
            this->sprite->setPosition(sf::Vector2f(-100, -100));
        }
    }
    else {
        this->sprite->setPosition(sf::Vector2f(xCoordonate * 82, yCoordonate + yCoordonate * 63));
    }
}

void ElementDisplayer::setElementSpritePosition(unsigned int rank, std::array<int, 2> hexSize)
{
    int xCoordonate = rank % 15;
    int yCoordonate = rank / 15;

    int xOffset = (hexSize.at(0) - this->sprite->getLocalBounds().width) / 2;
    int yOffset = (hexSize.at(1) - this->sprite->getLocalBounds().height) / 2;

    if (yCoordonate%2==0) {
        if (xCoordonate !=14 ){
            this->sprite->setPosition(sf::Vector2f(xOffset + 41 + xCoordonate * 82, yOffset + yCoordonate + yCoordonate * 63));
        }
        else{
            this->sprite->setPosition(sf::Vector2f(-100, -100));
        }
    }
    else {
        this->sprite->setPosition(sf::Vector2f(xOffset + xCoordonate * 82, yOffset +  yCoordonate + yCoordonate * 63));
    }
}

std::string ElementDisplayer::getType()
{
    return this->pathElement;
}

}
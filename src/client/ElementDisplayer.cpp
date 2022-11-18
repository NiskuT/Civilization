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

std::string ElementDisplayer::getType()
{
    return this->pathElement;
}

}
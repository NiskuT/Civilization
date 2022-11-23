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

void TextureDisplayer::setElementSpritePosition(std::array<int, 2> hexSize, int index, int rank)
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
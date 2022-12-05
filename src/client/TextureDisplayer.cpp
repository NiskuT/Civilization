#include <client.hpp>
#include <iostream>

namespace client {
    
TextureDisplayer::TextureDisplayer(const std::string& filename)
{    
    std::unique_ptr<sf::Texture> resource(new sf::Texture());
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("Holder::load - Failed to load " + filename);

    texture = std::move(resource);
}

void TextureDisplayer::addMapSprite()
{
    std::unique_ptr<sf::Sprite> sprite(new sf::Sprite());
    sprite->setTexture(*texture);
    sprites.push_back(std::move(sprite));
}

void TextureDisplayer::setMapSpritePosition(int index, int rank)
{
    int xCoordonate = rank % 15;
    int yCoordonate = rank / 15;

    if (yCoordonate%2==0) {
        if (xCoordonate !=14 ){
            this->sprites[index]->setPosition(sf::Vector2f(41 + xCoordonate * 82, yCoordonate + yCoordonate * 63));
        }
        else{
            this->sprites[index]->setPosition(sf::Vector2f(-100, -100));
        }
    }
    else {
        this->sprites[index]->setPosition(sf::Vector2f(xCoordonate * 82, yCoordonate + yCoordonate * 63));
    }
}

void TextureDisplayer::setElementSpritePosition(int index, int rank, std::array<int, 2> hexSize)
{
    int xCoordonate = rank % 15;
    int yCoordonate = rank / 15;

    int xOffset = (hexSize.at(0) - this->sprites[index]->getLocalBounds().width) / 2;
    int yOffset = (hexSize.at(1) - this->sprites[index]->getLocalBounds().height) / 2;

    if (yCoordonate%2==0) {
        if (xCoordonate !=14 ){
            this->sprites[index]->setPosition(sf::Vector2f(xOffset + 41 + xCoordonate * 82, yOffset + yCoordonate + yCoordonate * 63));
        }
        else{
            this->sprites[index]->setPosition(sf::Vector2f(-100, -100));
        }
    }
    else {
        this->sprites[index]->setPosition(sf::Vector2f(xOffset + xCoordonate * 82, yOffset +  yCoordonate + yCoordonate * 63));
    }
}

unsigned TextureDisplayer::getSize()
{    
    return sprites.size();
}

sf::Sprite& TextureDisplayer::getSprite(unsigned index)
{    
    return *sprites[index];
}

int TextureDisplayer::getWidth()
{    
    return getSize() > 0 ? this->sprites[0]->getLocalBounds().width : 0;
}

int TextureDisplayer::getHeight()
{    
    return getSize() > 0 ? this->sprites[0]->getLocalBounds().height : 0;
}

}
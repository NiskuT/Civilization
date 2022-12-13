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

void TextureDisplayer::setSpritePosition(int index, int x, int y, int xOffset, int yOffset, std::array<int, 2> hexSize)
{
    xOffset += hexSize.at(0) != 0 ? (hexSize.at(0) - this->sprites.at(index)->getLocalBounds().width) / 2 : 0;
    yOffset += hexSize.at(1) != 0 ? (hexSize.at(1) - this->sprites.at(index)->getLocalBounds().height) / 2 : 0;

    if (y%2==0) {
        if (x !=14 ){
            x = xOffset + 41 + x * 82;
            y = yOffset + y + y * 63;
        }
        else{
            x = -100;
            y = -100;
        }
    }
    else {
        x = xOffset + x * 82;
        y = yOffset + y + y * 63;
    }
    this->sprites.at(index)->setPosition(sf::Vector2f(x, y));
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
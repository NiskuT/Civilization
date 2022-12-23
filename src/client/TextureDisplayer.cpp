#include <client.hpp>
#include <iostream>

#define  FIRST_OFFSET_SCALE  (float(185) / float(1600))       // Offset of the first priority card 
#define  PRIORITY_CARD_OFFSET (float(249) / float(1600))      // Offset between each card
#define  PROPORTION_WIDTH  (float(140) / float(900))         

namespace client {
    
TextureDisplayer::TextureDisplayer(const std::string& filename)
{    
    std::unique_ptr<sf::Texture> resource(new sf::Texture());
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("Holder::load - Failed to load " + filename);

    texture = std::move(resource);
}

void TextureDisplayer::setImageType(HudTextureType imageType)
{
    this->typeOfImageToLoad = imageType;
}

/*!
 * \brief Add a Sprite with the texture to the TextureDisplayer
 */
void TextureDisplayer::addMapSprite()
{
    std::unique_ptr<sf::Sprite> sprite(new sf::Sprite());
    sprite->setTexture(*texture);
    sprites.push_back(std::move(sprite));
}

/*!
 * \brief Move the sprite Position
 * @param index is the index position of the sprite
 * @param xOffset is the X offset of the map in the screen
 * @param yOffset is the Y offset of the map in the screen
 */
void TextureDisplayer::moveSpritePosition(int xOffset, int yOffset)
{
    for (unsigned i = 0; i < this->sprites.size(); i++){
        sf::Vector2f pos = getSprite(i).getPosition();
        getSprite(i).setPosition(pos.x + xOffset, pos.y + yOffset);

    }
}

/*!
 * \brief Set a particular Sprite Position
 * @param index is the index position of the sprite
 * @param x is the X position of the sprite in the Map
 * @param y is the Y position of the sprite in the Map
 * @param xOffset is the X offset of the map in the screen
 * @param yOffset is the Y offset of the map in the screen
 * @param hexSize is the size of an hexagon, it is used to place element on the Map
 */
void TextureDisplayer::setSpritePosition(int index, int x, int y, int xOffset, int yOffset, std::array<int, 2> hexSize)
{
    xOffset += hexSize[0] != 0 ? (hexSize[0] - getSprite(index).getLocalBounds().width) / 2 : 0;
    yOffset += hexSize[1] != 0 ? (hexSize[1] - getSprite(index).getLocalBounds().height) / 2 : 0;

    int xHexSize = hexSize[0] != 0 ? hexSize[0] : getSprite(index).getLocalBounds().width;
    int yHexSize = hexSize[1] != 0 ? hexSize[1] : getSprite(index).getLocalBounds().height;

    if (y%2==0) {
        //if (x !=14 ){
            // divide by 2 to have the good offset & -1 because width count the size from 1
            x = xOffset + (int)(xHexSize)/2  + x * (xHexSize - 1);

            // multiply by 3/4 to not count the supperposition of hexagon & -1 because height count the size from 1
            y = yOffset + y + y * (yHexSize - 1) * 3 / 4;
        /*}
        else{
            x = -100;
            y = -100;
        }*/
    }
    else {
        x = xOffset + x * (xHexSize - 1);
        y = yOffset + y + y * (yHexSize - 1) * 3 / 4;
    }
    getSprite(index).setPosition(sf::Vector2f(x, y));
}

void TextureDisplayer::setHudSpritePosition(float scale, int windowLength, int windowWidth, int rotation, int priorityCardIndex)
{
    int xPos = 0;
    int yPos = 0;

    switch (this->typeOfImageToLoad)
    {

    case HudTextureType::barbareWheel0:
    case HudTextureType::barbareWheel1:
    case HudTextureType::barbareWheel2:
    case HudTextureType::barbareWheel3:
    case HudTextureType::barbareWheel4:
    {
        yPos = (windowWidth - getWidth() * scale);
        break;
    }

    case HudTextureType::ladder:
    {
        xPos = (windowLength - getWidth() * scale) / 2;
        yPos = (windowWidth - getHeight() * scale);
        break;
    }

    case HudTextureType::techWheel:
    {
        xPos = windowLength;
        yPos = windowWidth;
        sprites[0]->setOrigin(getWidth() / 2, getHeight() / 2);
        sprites[0]->rotate(rotation);
        break;
    }

    case HudTextureType::priorityCardEconomy:
    case HudTextureType::priorityCardArmy:
    case HudTextureType::priorityCardScience:
    case HudTextureType::priorityCardCulture:
    case HudTextureType::priorityCardIndustry:
    {
        xPos = PRIORITY_CARD_OFFSET * windowLength * priorityCardIndex + FIRST_OFFSET_SCALE * windowLength;
        yPos = windowWidth - getHeight() * scale + PROPORTION_WIDTH * windowWidth;
        break;
    }

    case HudTextureType::empty:
    break;

    default:
        std::cerr << "Error loading Image Type" << std::endl;
        break;
    }

    getSprite().setScale(scale, scale);
    getSprite().setPosition(xPos, yPos);

}

/*!
 * \brief Get the number of sprite in a TextureDisplayer
 */
unsigned TextureDisplayer::getSize()
{    
    return sprites.size();
}
/*!
 * \brief Get a particular sprite
 *
 * @param index is the position of the sprite in the textureDisplayer list of Sprite
 */
sf::Sprite& TextureDisplayer::getSprite(unsigned index)
{    
    return *sprites[index];
}
/*!
 * \brief Get the Width of the texture
 */
int TextureDisplayer::getWidth()
{    
    return getSize() > 0 ? getSprite().getLocalBounds().width : 0;
}
/*!
 * \brief Get the Height of the texture
 */
int TextureDisplayer::getHeight()
{    
    return getSize() > 0 ? getSprite().getLocalBounds().height : 0;
}

}

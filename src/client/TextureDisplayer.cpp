#include <client.hpp>
#include <iostream>

namespace client {

/*!
 * \brief Operator
 *
 *Return the texture of the TextureDisplayer
 */
sf::Texture TextureDisplayer::operator()()
{
   return *this->texture;
}

/*!
 * \brief Constructor
 *
 * Constructor of TextureDisplayer class
 *
 * @param path enter the path to the png you want to use
 */
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
    for(int i = 0; i < (int)pathImage.size(); i++){
        if (pathImage[i] == ('/')){
            nameStartPosition = i;
        }
    }

    this->type = pathImage.substr(nameStartPosition + 1, pathImage.size() - nameStartPosition - 5);
}
/*!
 * \brief Destructor
 */
TextureDisplayer::~TextureDisplayer()
{    
    //delete this->texture;
}

/*!
 * \brief Add a Sprite with the texture to the TextureDisplayer
 */
void TextureDisplayer::addMapSprite()
{
    sf::Sprite* newSprite = new sf::Sprite(*(this->texture));
    this->sprites.push_back(*newSprite);
}

/*!
 * \brief Set a particular Sprite Position
 * @param x is the X position of the sprite in the Map
 * @param y is the Y position of the sprite in the Map
 * @param xOffset is the X offset of the map in the screen
 * @param yOffset is the Y offset of the map in the screen
 * @param hexSize is the size of an hexagon, it is used to place element on the Map
 */
void TextureDisplayer::setSpritePosition(int index, int x, int y, int xOffset, int yOffset, std::array<int, 2> hexSize)
{
    xOffset += hexSize.at(0) != 0 ? (hexSize.at(0) - this->sprites.at(index).getLocalBounds().width) / 2 : 0;
    yOffset += hexSize.at(1) != 0 ? (hexSize.at(1) - this->sprites.at(index).getLocalBounds().height) / 2 : 0;

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
    this->sprites.at(index).setPosition(sf::Vector2f(x, y));
}


void TextureDisplayer::setHudSpritePosition(float scale, int windowLength, int windowWidth, int rotation, int priorityCardIndex)
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
        sprites[0].setOrigin(getWidth()/2, getHeight()/2);
        sprites[0].rotate(rotation);
    }

      else if (this->type == "barbare-wheel-0" || this->type == "barbare-wheel-1" || this->type == "barbare-wheel-2" || this->type == "barbare-wheel-3" || this->type == "barbare-wheel-4") {
        yPos = (windowWidth-getWidth()*scale);
        xPos= 0;
    }


    else if (this->type.substr(0, 8) == "priority") {
        float fisrtLengthOffsetScale = float(185) / float(1600); // Offset of the first priority card
        float priorityCardOffset = float(249) / float(1600); // Offset between each card
        float proportionWidth = float(140) / float(900);
        xPos = priorityCardOffset*windowLength*priorityCardIndex + fisrtLengthOffsetScale*windowLength; 
        yPos = windowWidth-getHeight()*scale + proportionWidth*windowWidth; 

    }


    else if (this->type.substr(0, 11) == "action-card") {
        int actionCardNumber = 0;
        float rightOffset = float(10) / float(1600);
        float upOffset = float(900/4) / float(900);
        xPos = windowLength - getWidth()*scale - rightOffset*windowLength;
        yPos = upOffset*windowWidth + (getHeight()*scale+10)*actionCardNumber;
    }



    this->sprites.at(0).setScale(scale, scale);
    this->sprites.at(0).setPosition(xPos, yPos);
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
sf::Sprite* TextureDisplayer::getSprite(unsigned index)
{
    return &sprites.at((int)(index));
}

/*!
 * \brief Get the Width of the texture
 */
int TextureDisplayer::getWidth()
{    
    return getSize() > 0 ? this->sprites.at(0).getLocalBounds().width : 0;
}

/*!
 * \brief Get the Height of the texture
 */
int TextureDisplayer::getHeight()
{    
    return getSize() > 0 ? this->sprites.at(0).getLocalBounds().height : 0;
}

}
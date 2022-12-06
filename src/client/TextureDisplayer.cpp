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
    for(unsigned i = 0; i < pathImage.size(); i++){
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
 * @param index is the index position of the sprite
 * @param x is the X position of the sprite in the Map
 * @param y is the Y position of the sprite in the Map
 * @param xOffset is the X offset of the map in the screen
 * @param yOffset is the Y offset of the map in the screen
 * @param hexSize is the size of an hexagon, it is used to place element on the Map
 */
void TextureDisplayer::setSpritePosition(int index, int x, int y, int xOffset, int yOffset, std::array<int, 2> hexSize)
{
    xOffset += hexSize[0] != 0 ? (hexSize[0] - this->sprites.at(index).getLocalBounds().width) / 2 : 0;
    yOffset += hexSize[1] != 0 ? (hexSize[1] - this->sprites.at(index).getLocalBounds().height) / 2 : 0;

    int xHexSize = hexSize[0] != 0 ? hexSize[0] : this->sprites.at(index).getLocalBounds().width;
    int yHexSize = hexSize[1] != 0 ? hexSize[1] : this->sprites.at(index).getLocalBounds().height;

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
    this->sprites.at(index).setPosition(sf::Vector2f(x, y));
}

/*!
 * \brief Move the sprite Position
 * @param index is the index position of the sprite
 * @param xOffset is the X offset of the map in the screen
 * @param yOffset is the Y offset of the map in the screen
 */
void TextureDisplayer::mooveSpritePosition(int xOffset, int yOffset)
{
    for (unsigned i = 0; i < this->sprites.size(); i++){
        sf::Vector2f pos = this->sprites[i].getPosition();
        this->sprites[i].setPosition(pos.x + xOffset, pos.y + yOffset);

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
        sprites[0].setOrigin(getWidth()/2, getHeight()/2);
        sprites[0].rotate(rotation);
    }

      else if (this->type == "barbare-wheel-0" || this->type == "barbare-wheel-1" || this->type == "barbare-wheel-2" || this->type == "barbare-wheel-3" || this->type == "barbare-wheel-4") {
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

    this->sprites[0].setScale(scale, scale);
    this->sprites[0].setPosition(xPos, yPos);
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
    return getSize() > 0 ? this->sprites[0].getLocalBounds().width : 0;
}
/*!
 * \brief Get the Height of the texture
 */
int TextureDisplayer::getHeight()
{    
    return getSize() > 0 ? this->sprites[0].getLocalBounds().height : 0;
}

}
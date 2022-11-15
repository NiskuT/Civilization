#include <client.hpp>
#include <iostream>

#define Y_OFFSET -21

namespace client {
    
/*!
* \brief Constructeur
*
* Constructor of TextureData class
*
* \param yCoord: y coordonate of the texture 
* xCoord: x coordonate of the texture 
* yOff: offset on y coordonate of the texture 
* xOff: offset on x coordonate of the texture 
* file: path of the texture image 
*/
TextureData::TextureData(int yCoord, int xCoord, int yOff, int xOff, std::string file)
{
    elementCoord.at(0) = xCoord;
    elementCoord.at(1) = yCoord;
    elementOffset.at(0) = xOff;
    elementOffset.at(1) = yOff;
    fileName = file;
}

/*!
* \brief Load the texture and set the position on the screen
*/
void TextureData::loadTexture() 
{
    if (!texture.loadFromFile(fileName))
    {
        std::cout << "Error loading element picture: " << fileName << "\n";
    };
    setTexturePosition({0, 0});
}

/*!
* \brief Add an offset to the element
*/
void TextureData::updateElementPosition(std::array<int, 2> mapOffset) 
{
    setTexturePosition({mapOffset.at(0), mapOffset.at(1)});
}

/*!
* \brief Method to calcule the position on the map
*/
void TextureData::setTexturePosition(std::array<int, 2> Offset)
{
    int x_Offset = elementOffset.at(0);
    if (elementCoord.at(1)%2==0) x_Offset += 41;

    position.at(0) = Offset.at(0) + x_Offset + elementCoord.at(0) * 83;
    position.at(1) = Offset.at(1) + elementOffset.at(1) + elementCoord.at(1) * (85 + Y_OFFSET);
}

}
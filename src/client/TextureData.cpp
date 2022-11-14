#include <client.hpp>
#include <iostream>

#define Y_OFFSET -21

namespace client {
    
TextureData::TextureData(int yCoord, int xCoord, int yOff, int xOff, std::string file)
{
    xCoordonne = xCoord;
    yCoordonne = yCoord;
    xOffset = xOff;
    yOffset = yOff;
    fileName = file;
}
void TextureData::loadTexture() {
    if (!texture.loadFromFile(fileName))
    {
        std::cout << "Error loading element picture \n";
    };
    int x_Offset = xOffset;
    if (yCoordonne%2==0) x_Offset += 41;
    position.at(0) = x_Offset + xCoordonne * 83;
    position.at(1) = yOffset + yCoordonne * (85 + Y_OFFSET);
    }
};
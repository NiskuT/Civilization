#include <shared.hpp>

namespace client {

void TextureData::load(int yCoordonne, 
int xCoordonne, int yOffset, int xOffset, std::string fileName)
{
    this->xCoordonne = xCoordonne;
    this->yCoordonne = yCoordonne;
    this->xOffset = xOffset;
    this->yOffset = yOffset;
    this->fileName = fileName;
}

int TextureData::getxCoord() 
{
    return this->xCoordonne;
}

int TextureData::getyCoord() 
{
    return this->yCoordonne;
}

int TextureData::getxOffset() 
{
    return this->xOffset;
}

int TextureData::getyOffset() 
{
    return this->yOffset;
}

std::string TextureData::getFileName() 
{
    return this->fileName;
}

}
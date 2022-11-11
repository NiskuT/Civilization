#include <shared.hpp>

namespace shared {

void ElementData::update(int yCoordonne, int xCoordonne, int yOffset, int xOffset, std::string fileName)
{
    this->xCoordonne = xCoordonne;
    this->yCoordonne = yCoordonne;
    this->xOffset = xOffset;
    this->yOffset = yOffset;
    this->fileName = fileName;
};

int ElementData::getxCoord() {return xCoordonne;};
int ElementData::getyCoord() {return yCoordonne;};
int ElementData::getxOffset() {return xOffset;};
int ElementData::getyOffset() {return yOffset;};
std::string ElementData::getFileName() {return fileName;};

}
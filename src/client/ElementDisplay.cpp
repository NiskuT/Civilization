#include <client.hpp>

#define Y_OFFSET -21

namespace client {

void ElementDisplay::loadTexture(client::TextureData &data) {
    if (!texture.loadFromFile("../ressources/img/map/player-2.png"/*data.fileName*/));
    int x_Offset = data.xOffset;
    if (data.yCoordonne%2==0) x_Offset += 41;
    position.at(0) = x_Offset + data.xCoordonne * 83;
    position.at(1) = data.yOffset + data.yCoordonne * (85 + Y_OFFSET);
    }
};
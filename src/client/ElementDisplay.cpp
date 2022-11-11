#include <client.hpp>

#define Y_OFFSET -21

namespace client {

void ElementDisplay::loadTexture(client::TextureData &data) {
    if (!texture.loadFromFile(data.getFileName()));
    int x_Offset = data.getxOffset();
    if (data.getyCoord()%2==0) x_Offset += 41;
    position[0] = x_Offset + data.getxCoord() * 83;
    position[1] = data.getyOffset() + data.getyCoord() * (85 + Y_OFFSET);
    }
};
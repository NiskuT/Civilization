#include <client.hpp>
#include <iostream>

#define Y_OFFSET -21
#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0

namespace client {

HexDisplay::HexDisplay()
{
    int level[165] =
    {
        0, 0, 5, 4, 4, 2, 5, 2, 1, 2, 5, 0, 0, 0, 0,
        0, 0, 0, 5, 0, 1, 5, 5, 5, 5, 1, 1, 0, 0, 0,
        0, 0, 0, 3, 0, 1, 5, 0, 1, 1, 3, 4, 0, 0, 0,
        0, 0, 0, 0, 1, 2, 5, 3, 2, 3, 1, 4, 0, 0, 0,
        0, 0, 0, 0, 4, 5, 5, 2, 3, 0, 3, 2, 0, 0, 0,
        0, 0, 0, 0, 2, 5, 1, 3, 3, 2, 2, 5, 0, 0, 0,
        0, 0, 5, 1, 2, 1, 5, 3, 1, 5, 2, 5, 4, 0, 0,
        0, 0, 0, 1, 2, 0, 4, 2, 5, 3, 1, 4, 5, 0, 0,
        0, 0, 0, 0, 2, 0, 5, 4, 2, 0, 0, 3, 2, 2, 0,
        0, 0, 0, 0, 1, 1, 5, 3, 0, 5, 2, 4, 2, 2, 0,
        0, 0, 0, 0, 0, 1, 5, 5, 0, 1, 0, 0, 4, 2, 0,
    };

    for(unsigned i = 0; i < mapLevel.size(); i++){
        mapLevel.at(i) = level[i];
    }
}

void HexDisplay::loadHexTexture(const std::string& fileName, sf::Vector2u hexSize, unsigned int width, unsigned int height)
{
    // load the fileName texture
    if (!m_fileName.loadFromFile(fileName)){
        std::cout << "Error loading hex picture \n";
    }

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    //used to know where the line start (pair line have an offset of 41px)
    unsigned int offset;

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i){
        for (unsigned int j = 0; j < height; ++j){

            if (j%2==0) offset = 41;
            else offset = 0;

            if ((offset == 0) | (i+1 != width))
            {
                // get the current tile number
                int tileNumber = mapLevel.at(i + j * width);

                // find its position in the fileName texture
                int tu = tileNumber % (m_fileName.getSize().x / hexSize.x);
                int tv = tileNumber / (m_fileName.getSize().x / hexSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(offset + i * hexSize.x, j * (Y_OFFSET + hexSize.y));
                quad[1].position = sf::Vector2f(offset + (i + 1) * hexSize.x, j * (Y_OFFSET + hexSize.y));
                quad[2].position = sf::Vector2f(offset + (i + 1) * hexSize.x, j * (Y_OFFSET + hexSize.y) + hexSize.y);
                quad[3].position = sf::Vector2f(offset + i * hexSize.x, j * (Y_OFFSET + hexSize.y) + hexSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * hexSize.x, tv * hexSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * hexSize.x, tv * hexSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * hexSize.x, (tv + 1) * hexSize.y);
                quad[3].texCoords = sf::Vector2f(tu * hexSize.x, (tv + 1) * hexSize.y);
            }
        }
    }
}
        
void HexDisplay::updateHexPosition(sf::Vector2u hexSize, unsigned int width, unsigned int height)
{

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    unsigned int offset;

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            if (j%2==0) offset = 41;
            else offset = 0;

            if ((offset == 0) | (i+1 != width))
            {
                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(MAP_X_OFFSET + offset + i * hexSize.x, MAP_Y_OFFSET + j * (Y_OFFSET + hexSize.y));
                quad[1].position = sf::Vector2f(MAP_X_OFFSET + offset + (i + 1) * hexSize.x, MAP_Y_OFFSET + j * (Y_OFFSET + hexSize.y));
                quad[2].position = sf::Vector2f(MAP_X_OFFSET + offset + (i + 1) * hexSize.x, MAP_Y_OFFSET + j * (Y_OFFSET + hexSize.y) + hexSize.y);
                quad[3].position = sf::Vector2f(MAP_X_OFFSET + offset + i * hexSize.x, MAP_Y_OFFSET + j * (Y_OFFSET + hexSize.y) + hexSize.y);
            }
        }
}

void HexDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the fileName texture
    states.texture = &m_fileName;

    // draw the vertex array
    target.draw(m_vertices, states);
}

}

#include <client.hpp>
#include <iostream>

#define Y_OFFSET -21
#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0

namespace client {

void HexDisplay::load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        std::cout << "Error loading Hex file";

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
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(offset + i * tileSize.x, j * (Y_OFFSET + tileSize.y));
                quad[1].position = sf::Vector2f(offset + (i + 1) * tileSize.x, j * (Y_OFFSET + tileSize.y));
                quad[2].position = sf::Vector2f(offset + (i + 1) * tileSize.x, j * (Y_OFFSET + tileSize.y) + tileSize.y);
                quad[3].position = sf::Vector2f(offset + i * tileSize.x, j * (Y_OFFSET + tileSize.y) + tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
        }
}
        
void HexDisplay::update(sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
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
                quad[0].position = sf::Vector2f(MAP_X_OFFSET + offset + i * tileSize.x, MAP_Y_OFFSET + j * (Y_OFFSET + tileSize.y));
                quad[1].position = sf::Vector2f(MAP_X_OFFSET + offset + (i + 1) * tileSize.x, MAP_Y_OFFSET + j * (Y_OFFSET + tileSize.y));
                quad[2].position = sf::Vector2f(MAP_X_OFFSET + offset + (i + 1) * tileSize.x, MAP_Y_OFFSET + j * (Y_OFFSET + tileSize.y) + tileSize.y);
                quad[3].position = sf::Vector2f(MAP_X_OFFSET + offset + i * tileSize.x, MAP_Y_OFFSET + j * (Y_OFFSET + tileSize.y) + tileSize.y);
            }
        }
}

void HexDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

}

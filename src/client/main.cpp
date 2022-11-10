#include <iostream>
#include <client.hpp>
#include <SFML/Graphics.hpp>

#define Y_OFFSET -21

class  Element {
    // Attributes
  public:
    int position[2] = {0, 0};
    sf::Texture texture;
    // Operations
  public:
    void update(std::string nameFile, int x, int y) {
        if (!texture.loadFromFile(nameFile))
        {
            //Error
        }
        int x_offset;
        if (x%2==0) x_offset = 41;
        else x_offset = 0;
        position[1] = x * (85 + Y_OFFSET); position[0] = x_offset + y * 83;
    }
  };

class FieldMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        unsigned int offset = 0;

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

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

int main(int argc,char* argv[])
{
    
    std::string arg;
    if (argc == 2) client::sayHello sayHello(argv[1]);

    else {
        std::cout << "Unkown command, use -h for help." << std::endl;
    }

    // create the window
    sf::RenderWindow window(sf::VideoMode(1245, 725), "Civilization VII");

    // define the level with an array of tile indices
    const int level[] =
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

    // create the tilemap from the level definition
    FieldMap fieldMap;
    if (!fieldMap.load("../ressources/img/map/field.png", sf::Vector2u(83, 85), level, 15, 11))
        return -1;

    int lenElement = 5;

    const int positions[lenElement][2] =
    {
        {0,4},
        {2,7},
        {6,8},
        {7,13},
        {10,6}
    };

    const int offset[lenElement][2] =
    {
        {5,1},
        {5,1},
        {5,1},
        {5,1},
        {5,1},
        {5,1}
    };

    const std::string nameFile[lenElement] =
    {
        "../ressources/img/map/ressource-oil.png",
        "../ressources/img/map/ressource-stone.png",
        "../ressources/img/map/ressource-antic.png",
        "../ressources/img/map/ressource-stone.png",
        "../ressources/img/map/ressource-diamond.png"
    };

    sf::Sprite elementSprites[lenElement];
    Element elements[lenElement];

    for(int i =0; i < lenElement; i++)
    {
        elements[i].update(nameFile[i], positions[i][0], positions[i][1]);
        elementSprites[i].setTexture(elements[i].texture);
        elementSprites[i].move(sf::Vector2f(elements[i].position[0] * 1.f, elements[i].position[1] * 1.f));
    }

    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // draw the map
        window.clear();
        window.draw(fieldMap);
        for(int i =0; i < lenElement; i++)
        {
            window.draw(elementSprites[i]);
        }
        window.display();
    }

    return 0;

}

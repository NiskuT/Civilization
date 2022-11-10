#include <iostream>
#include <client.hpp>
#include <SFML/Graphics.hpp>

#define Y_OFFSET -21
#define LEN_ELEMENT 43

class ElementData {
    public:
        int xCoordonne;
        int yCoordonne;
        int xOffset;
        int yOffset;
        std::string fileName;
        
        void update(int yCoordonne, int xCoordonne, int yOffset, int xOffset, std::string fileName)
        {
            this->xCoordonne = xCoordonne;
            this->yCoordonne = yCoordonne;
            this->xOffset = xOffset;
            this->yOffset = yOffset;
            this->fileName = fileName;
        }
        int getxCoord() {return xCoordonne;};
        int getyCoord() {return yCoordonne;};
        int getxOffset() {return xOffset;};
        int getyOffset() {return yOffset;};
        std::string getFileName() {return fileName;};
};

class  Element {
    // Attributes
  public:
    int position[2] = {0, 0};
    sf::Texture texture;
    // Operations
  public:
    void update(ElementData data) {
        if (!texture.loadFromFile(data.getFileName()))
        {
            //Error
        }
        int xOffset = data.getxOffset();
        if (data.getyCoord()%2==0) xOffset += 41;
        position[0] = xOffset + data.getxCoord() * 83;
        position[1] = data.getyOffset() + data.getyCoord() * (85 + Y_OFFSET);
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

class  GameWindow {
    
  public:

    // create the window
    sf::RenderWindow window;

    // define the level with an array of tile indices
    const int level[165] =
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
        
    ElementData elementData[LEN_ELEMENT];
    Element elements[LEN_ELEMENT];
    sf::Sprite elementSprites[LEN_ELEMENT];
    //shared::element elements[LEN_ELEMENT];
    
  public:
    GameWindow() {

        window.create(sf::VideoMode(1245, 725),"Civilization VII");

        
        if (!fieldMap.load("../ressources/img/map/field.png", sf::Vector2u(83, 85), level, 15, 11))
            std::cout << "Error loading field";

        elementData[0].update(0,4,20,21,"../ressources/img/map/ressource-oil.png");
        elementData[1].update(2,6,20,21,"../ressources/img/map/ressource-stone.png");
        elementData[2].update(6,7,20,21,"../ressources/img/map/ressource-antic.png");
        elementData[3].update(7,11,20,21,"../ressources/img/map/ressource-stone.png");
        elementData[4].update(10,6,20,21,"../ressources/img/map/ressource-diamond.png");
        elementData[5].update(1,8,20,22,"../ressources/img/map/player-2.png");
        elementData[6].update(2,9,20,22,"../ressources/img/map/player-2.png");
        elementData[7].update(2,10,20,22,"../ressources/img/map/player-2.png");
        elementData[8].update(4,10,20,22,"../ressources/img/map/player-2.png");
        elementData[9].update(5,11,20,22,"../ressources/img/map/player-2.png");
        elementData[10].update(1,9,20,22,"../ressources/img/map/player-2-reinforced.png");
        elementData[11].update(3,9,20,22,"../ressources/img/map/player-2-reinforced.png");
        elementData[12].update(3,8,20,22,"../ressources/img/map/player-2-reinforced.png");
        elementData[13].update(5,9,20,22,"../ressources/img/map/player-2-reinforced.png");
        elementData[14].update(6,10,20,22,"../ressources/img/map/player-2-reinforced.png");
        elementData[15].update(2,8,10,9,"../ressources/img/map/player-2-mature.png");
        elementData[16].update(3,10,10,9,"../ressources/img/map/player-2-capital.png");
        elementData[17].update(4,8,10,9,"../ressources/img/map/player-2-city.png");
        elementData[18].update(5,10,10,9,"../ressources/img/map/player-2-city.png");
        elementData[19].update(8,11,17,17,"../ressources/img/map/player-2-caravan.png");
        elementData[20].update(5,7,20,22,"../ressources/img/map/player-1.png");
        elementData[21].update(6,5,20,22,"../ressources/img/map/player-1.png");
        elementData[22].update(6,2,20,22,"../ressources/img/map/player-1.png");
        elementData[23].update(7,3,20,22,"../ressources/img/map/player-1.png");
        elementData[24].update(7,7,20,22,"../ressources/img/map/player-1.png");
        elementData[25].update(6,6,20,22,"../ressources/img/map/player-1-reinforced.png");
        elementData[26].update(4,5,20,22,"../ressources/img/map/player-1-reinforced.png");
        elementData[27].update(6,4,20,22,"../ressources/img/map/player-1-reinforced.png");
        elementData[28].update(8,6,20,22,"../ressources/img/map/player-1-reinforced.png");
        elementData[29].update(5,6,10,9,"../ressources/img/map/player-1-capital.png");
        elementData[30].update(7,6,10,9,"../ressources/img/map/player-1-mature.png");
        elementData[31].update(6,3,10,9,"../ressources/img/map/player-1-city.png");
        elementData[32].update(7,9,17,17,"../ressources/img/map/player-1-caravan.png");
        elementData[33].update(7,10,10,9,"../ressources/img/map/state-city-science.png");
        elementData[34].update(9,12,10,9,"../ressources/img/map/state-city-culture.png");
        elementData[35].update(9,4,10,9,"../ressources/img/map/state-city-army.png");
        elementData[36].update(0,3,20,22,"../ressources/img/map/barbarians.png");
        elementData[37].update(1,6,20,22,"../ressources/img/map/barbarians.png");
        elementData[38].update(6,6,20,22,"../ressources/img/map/barbarians.png");
        elementData[39].update(8,4,20,22,"../ressources/img/map/barbarians.png");
        elementData[40].update(0,7,20,22,"../ressources/img/map/barbarian-hutte.png");
        elementData[41].update(2,3,20,22,"../ressources/img/map/barbarian-hutte.png");
        elementData[42].update(9,6,20,22,"../ressources/img/map/barbarian-hutte.png");

        for(int i =0; i < LEN_ELEMENT; i++)
        {
            elements[i].update(elementData[i]);
            elementSprites[i].setTexture(elements[i].texture);
            elementSprites[i].move(sf::Vector2f(elements[i].position[0] * 1.f, elements[i].position[1] * 1.f));
        }

    }

    void game() {

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
            display();
        }
    }

    void display() {
        window.clear();
        window.draw(fieldMap);
        for(int i =0; i < LEN_ELEMENT; i++)
        {
            window.draw(elementSprites[i]);
        }
        window.display();
    }
  };


int main(int argc,char* argv[])
{
    
    std::string arg;
    if (argc == 2) client::sayHello sayHello(argv[1]);

    else {
        std::cout << "Unkown command, use -h for help." << std::endl;
    }    

    GameWindow gameWindow;

    gameWindow.game();

    return 0;

}

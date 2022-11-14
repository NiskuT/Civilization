#include <iostream>
#include <client.hpp>
#include <SFML/Graphics.hpp>


class HudDisplay {
    private:
        sf::Sprite sprite;
        sf::Texture texture;
        std::string path;
        sf::Vector2<unsigned int> size;
        float scale;
        std::string type;

    public:
        HudDisplay(std::string path, float scale, std::string type ) {
            this->sprite = sprite;
            this->texture = texture;
            this->path = path;
            this->scale = scale;
            this->type = type;

            if (!texture.loadFromFile(path)){
                std::cout << "Error loading background";
            }
            
        sprite.setScale(scale, scale);
        sprite.setTexture(texture);
        
        }

        sf::Sprite getSprite() {return sprite;};

        void updatePlacement(int length, int width, int priorityCard){
        sf::Vector2<unsigned int> size = this->texture.getSize();
        int offsetLength = 0;
        int offsetWidth = 0;
        int rotation = 0; 
            if (this->type == "background") {
                
                offsetLength = -((size.x-length)/2);
                offsetWidth = -((size.y-width)/2);
            }

            else if (this->type == "ladder") {
                offsetLength = (length-size.x)/2;
                offsetWidth = (width-size.y);  
            }

            else if (this->type =="techWheel") {
                rotation = 90;
                this->sprite.setOrigin(size.x/2, size.y/2); // set the point of rotation at the center of the image
                offsetLength = length;
                offsetWidth = width; 
            }

            else if (this->type == "barbareWheel") {
                offsetWidth = (width-size.y*0.3);
            }

            else if (this->type == "priorityCard") {
                offsetLength = 10*priorityCard + 300;
                offsetWidth = width-size.y; 
            }

            else {
                std::cout << ("Not good type");
            }
            sprite.rotate(rotation);
            sprite.move(offsetLength, offsetWidth);
            
        }
};

int main(int argc,char* argv[])
{

    
    std::string arg;
    if (argc == 2) client::sayHello sayHello(argv[1]);

    else {
        std::cout << "Unkown command, use -h for help." << std::endl;
    }

    // create the window
    int length = 1600;
    int width = 900;
    sf::RenderWindow window(sf::VideoMode(length, width), "Civilization VII");


    // Display the background
    HudDisplay background("../ressources/img/hud/background.png", 1, "background");
    background.updatePlacement(length, width, 0);

    // Display ladder

    HudDisplay ladder("../ressources/img/hud/ladder.png",1 , "ladder");
    ladder.updatePlacement(length, width, 0);
    

    //Display tech wheel

    HudDisplay techWheel("../ressources/img/hud/tech-wheel.png", 0.3f, "techWheel");
    techWheel.updatePlacement(length,width, 0);

    // Display event dial (barbare wheel)

    int posBarbareWheel = 0;
    std::string file = "../ressources/img/hud/barbare-wheel-";
    std::string num = std::to_string(posBarbareWheel);
    std::string format= ".png";
    std::string fileToLoad = file + num + format;
    HudDisplay barbareWheel(fileToLoad, 0.3f, "barbareWheel");
    barbareWheel.updatePlacement(length, width, 0);


    // Display card

/*
    std::vector<std::string> cardType = {"army", "culture", "economy", "industry", "science"};
    std::vector<HudDisplay> priorityCards;
    for (int i=0; i<1; i++) {
        std::string file = "../ressources/img/hud/priority-card-";
        std::string type = cardType[i];
        std::string format= ".png";
        std::string fileToLoad = file + type + format;
        priorityCards.emplace_back(fileToLoad, 1, "priorityCard") ;
        priorityCards.back().updatePlacement(length, width, i);
    }
*/
   






/*
    // Display the map
    sf::Sprite map;
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../ressources/img/map/exemple.png"))
    {
        std::cout << "Error loading image";
    }
    map.setTexture(mapTexture);
    map.move(100*1.f, 100*1.f);
    map.setScale(0.5f, 0.5f);
*/


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
        window.draw(background.getSprite());
        window.draw(ladder.getSprite());
        window.draw(techWheel.getSprite());
        window.draw(barbareWheel.getSprite());
        //window.draw(map);
        window.display();
    }

    return 0;

}

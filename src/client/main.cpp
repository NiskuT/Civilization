#include <iostream>
#include <client.hpp>
#include <SFML/Graphics.hpp>

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
    sf::Sprite background;
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../ressources/img/hud/background.png"))
    {
        std::cout << "Error loading background";
    }
    background.setTexture(backgroundTexture);
    sf::Vector2<unsigned int> sizeBackground = backgroundTexture.getSize();
    int offsetBackground = (sizeBackground.y-width)/2; // move the background in width so that it is at the middle of the window
    background.move(0,-offsetBackground);

    // Display ladder

    sf::Sprite ladder;
    sf::Texture ladderTexture;
    if (!ladderTexture.loadFromFile("../ressources/img/hud/ladder.png"))
    {
        std::cout << "Error loading ladder";
    }
    ladder.setTexture(ladderTexture);
    sf::Vector2<unsigned int> sizeLadder = ladderTexture.getSize();
    int offsetWidthLadder = (width-sizeLadder.y);
    int offsetLengthLadder = (length-sizeLadder.x)/2;
    ladder.move(offsetLengthLadder,offsetWidthLadder);

    //Display tech wheel
    sf::Sprite techWheel;
    sf::Texture techWheelTexture;
    int rotation = 45;
    if (!techWheelTexture.loadFromFile("../ressources/img/hud/tech-wheel.png"))
    {
        std::cout << "Error loading tech wheele";
    }
    techWheel.setScale(0.3f, 0.3f);
    techWheel.setTexture(techWheelTexture);
    sf::Vector2<unsigned int> sizeTechWheel = techWheelTexture.getSize();
    techWheel.setOrigin(sizeTechWheel.x/2, sizeTechWheel.y/2); // set the point of rotation at the center of the image
    techWheel.move(length, width); // set the point of rotation at the bottom right
    techWheel.rotate(rotation);

    // Display event dial (barbare wheel)
    int posBarbareWheel = 0;

    sf::Sprite barbareWheel;
    sf::Texture barbareWheelTexture;
    std::string file = "../ressources/img/hud/barbare-wheel-";
    std::string num = std::to_string(posBarbareWheel);
    std::string format= ".png";
    std::string fileToLoad = file + num + format;
    if (!barbareWheelTexture.loadFromFile(fileToLoad))
    {
        std::cout << "Error loading barbare wheel";
    }
    barbareWheel.setScale(0.3f, 0.3f);
    barbareWheel.setTexture(barbareWheelTexture);
    sf::Vector2<unsigned int> sizeBarbareWheel = barbareWheelTexture.getSize();
    int offsetWidthBarbareWheel = (width-sizeBarbareWheel.y*0.3);
    barbareWheel.move(0, offsetWidthBarbareWheel);








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
        window.draw(background);
        window.draw(ladder);
        window.draw(techWheel);
        window.draw(barbareWheel);
        //window.draw(map);
        window.display();
    }

    return 0;

}

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

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
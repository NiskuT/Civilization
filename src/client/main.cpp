#include <iostream>
#include <client.hpp>
#include <shared.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, char *argv[])
{
    std::string arg;
    if (argc == 2)
        shared::sayHello sayHello(argv[1]);

    else
    {
        std::cout << "Unkown command, use -h for help." << std::endl;
    }

    client::GameWindow clientGame;
    clientGame.clientWindow();

    return 0;
}

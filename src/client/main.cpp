#include <iostream>
#include <client.hpp>
#include <shared.hpp>

int main(int argc, char *argv[])
{

    std::string arg;
    if (argc != 2)
    {
        std::cout << "Unkown command, use -h for help." << std::endl;
    }
    else
    {
        arg = argv[1];
        if (!arg.compare("-h"))
        {
            std::cout << "Usage: " << argv[0] << " [command]" << std::endl;
            std::cout << "Commands:" << std::endl;
            std::cout << "  -h\t\t\tShow this help" << std::endl;
            std::cout << "  -r\t\t\tStart client" << std::endl;
            std::cout << "  hello\t\t\tPrint hello world" << std::endl;
        }
        else if (arg.compare("-r") == 0 || arg.compare("render") == 0)
        {
            client::GameWindow clientGame;
            clientGame.clientWindow();
        }
        else if (arg.compare("hello") == 0)
        {
            std::cout << "Hello world" << std::endl;
        }
        else
        {
            std::cout << "Unkown command, use -h for help." << std::endl;
        }
    }
}

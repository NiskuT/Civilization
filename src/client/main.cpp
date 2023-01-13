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
            std::cout << "  -n\t\t\tStart network client" << std::endl;
        }
        else if (arg.compare("-r") == 0 || arg.compare("render") == 0)
        {
            client::ClientGameEngine clientGameEngine;
            clientGameEngine.renderGame();
            std::cout << "Thanks for playing our game !" << std::endl;
        }
        else if (arg.compare("hello") == 0)
        {
            std::cout << "Hello world" << std::endl;
        }
        else if (arg.compare("-n") == 0 || arg.compare("network") == 0)
        {
            client::ClientGameEngine maGame;
            maGame.connect("127.0.0.1", 8080);
            while(true)
            {
                sleep(3);
                std::unique_lock<std::mutex> lock(maGame.myself->qAndA.sharedDataMutex);
                maGame.myself->qAndA.question = "getstate\n";
                lock.unlock();
                maGame.askServer();

            }
        }
        else
        {
            std::cout << "Unkown command, use -h for help." << std::endl;
        }
    }
    exit(0);
}

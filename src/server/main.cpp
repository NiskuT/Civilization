#include <iostream>
#include <server.hpp>
#include <shared.hpp>

#define SERVER_VERSION "0.0.1"

int main(int argc,char* argv[])
{
    std::string arg;
    if (argc != 2) {
        std::cout << "Unkown command, use -h for help." << std::endl;
    }
    else {
        arg = argv[1];
        if (!arg.compare("-h")) {
            std::cout << "Usage: " << argv[0] << " [command]" << std::endl;
            std::cout << "Commands:" << std::endl;
            std::cout << "  -h\t\t\tShow this help" << std::endl;
            std::cout << "  -v\t\t\tShow version" << std::endl;
            std::cout << "  -s\t\t\tStart server" << std::endl;
            std::cout << "  -hello\t\t\tPrint hello world" << std::endl;
        }
        else if (!arg.compare("-v")) {
            std::cout << "Version: " << SERVER_VERSION << std::endl;
        }
        else if (!arg.compare("-s")) {
            server::Server server;
            server.start();
        }
        else if (!arg.compare("-hello")) {
            std::cout << "Hello world" << std::endl;
        }
        else {
            std::cout << "Unkown command, use -h for help." << std::endl;
        }
    }

    return 0;
}

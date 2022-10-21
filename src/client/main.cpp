#include <iostream>
#include <client.hpp>

int main(int argc,char* argv[])
{
    std::string arg;
    if (argc == 2) client::sayHello sayHello(argv[1]);

    else {
        std::cout << "Unkown command, use -h for help." << std::endl;
    }

   
    return 0;
}

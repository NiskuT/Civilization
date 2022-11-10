#include "client.hpp"
#include <iostream>


namespace client {

sayHello::sayHello(std::string arg) {
    
    int help = arg.compare(sayHello::commandList[0]);
    int h = arg.compare(sayHello::commandList[1]);
    int hello = arg.compare(sayHello::commandList[2]);

    if (!hello ) sayHello::helloOutput();
    else if (!help || !h) sayHello::helpOutput();
    else std::cout << "Unkown command, use -h for help." << std::endl;
}

void sayHello::helloOutput() {
    std::cout << "Hello world !" << std::endl;
}

void sayHello::helpOutput() {
    std::cout << "List of available command:" << std::endl;
    std::cout << "-h or -help : display this help" << std::endl;
    std::cout << "-hello : display hello world" << std::endl;
}


}
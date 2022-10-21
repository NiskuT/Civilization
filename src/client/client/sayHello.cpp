#include "sayHello.hpp"



sayHello::sayHello(string arg) {
    
    int help = arg.compare(sayHello::commandList[0]);
    int hello = arg.compare(sayHello::commandList[1]);

    if (!hello) sayHello::helloOutput();
    else if (!help) sayHello::helpOutput();
    else cout << "Unkown command, use -h for help." << std::endl;
}

void sayHello::helloOutput() {
    std::cout << "Hello world !" << std::endl;
}

void sayHello::help() {
    std::cout << "List of available command:" << std::endl;
    std::cout << "-h : display this help" << std::endl;
    std::cout << "-hello : display hello world" << std::endl;
}



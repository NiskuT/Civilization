#include <iostream>
#include <client.hpp>

int main(int argc,char* argv[])
{
    if (argc == 1) {
        client::sayHello("");
        return 0;
    }
    else {
        std::string arg = argv[1];
        client::sayHello(arg);
        return 0;
    }
    return 0;
}

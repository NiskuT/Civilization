#include <client.hpp>
#include <iostream>
#include <thread>
#include <sys/stat.h>

#define REFRESH_ELEMENT 1

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

namespace client
{

void ClientGameEngine::handleInformation(int x, int y)
{
    if (x == -1) {
        std::cout << "User click on the priority card " << y << std::endl;
    }
    else {
        std::cout << "User click on the Hex x=" << x << " & y=" << y << std::endl;
    }
}

void ClientGameEngine::startGameWindow(){
    clientGame.clientWindow([this](int x, int y) { handleInformation(x, y); });
}

void ClientGameEngine::renderGame() 
{
    std::thread t(&ClientGameEngine::startGameWindow, this);
    t.detach();

    long lastUpdateTimer = clientGame.getCurrentTime();
    struct stat file_stat;
    std::string file_path = RESOURCES_PATH "/img/map/files.json";
    if (stat(file_path.c_str(), &file_stat) == -1) {
        std::cerr << "Erreur lors de l'obtention des informations sur le fichier " << file_path << '\n';
    }
    time_t last_modified = file_stat.st_mtime;

    while(1){
        
        if (clientGame.getCurrentTime() - lastUpdateTimer > REFRESH_ELEMENT ){

            if (stat(file_path.c_str(), &file_stat) == -1) {
                std::cerr << "Erreur lors de l'obtention des informations sur le fichier " << file_path << '\n';
            }

            time_t modified = file_stat.st_mtime;
            lastUpdateTimer = clientGame.getCurrentTime();

            if (modified != last_modified){
                last_modified = modified;
                clientGame.updateElementTexture();
            }
        }
    }
}
    
}

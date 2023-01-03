#include <client.hpp>
#include <iostream>
#include <thread>

#define REFRESH_ELEMENT 1

#ifndef RESOURCES_PATH
    #define RESOURCES_PATH "../resources"
#endif

namespace client
{

void ClientGameEngine::renderGame() {

    client::GameWindow clientGame;
    clientGame.loadMapTexture();
    clientGame.loadElementTexture();
    clientGame.loadHudTexture();
    std::thread t(&GameWindow::clientWindow, &clientGame);

    int turn = 0;
    time_t currentTimer = time(NULL);
    time_t lastUpdateTimer = time(NULL);

    while(1){

        time(&currentTimer);
        if (currentTimer - lastUpdateTimer > REFRESH_ELEMENT ){
            //clientGame.loadElementTexture();
            time(&lastUpdateTimer);
            turn++;
        }

    }
}
    
}

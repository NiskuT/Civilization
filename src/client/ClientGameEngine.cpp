#include <client.hpp>
#include <iostream>
#include <thread>
#include <mutex>

#define REFRESH_ELEMENT 100

long getCurrentTime();

namespace client
{

void ClientGameEngine::renderGame() {

    client::GameWindow clientGame;
    std::thread t(&GameWindow::clientWindow, &clientGame);

    int turn = 0;

    long lastUpdateTimer = clientGame.getCurrentTime(false);

    while(1){

        if (clientGame.getCurrentTime(false) - lastUpdateTimer > REFRESH_ELEMENT ){
            std::lock_guard<std::mutex> lock(clientGame.mutexGame);
            clientGame.loadElementTexture();
            lastUpdateTimer = clientGame.getCurrentTime(false);
            turn++;
        }
    }
}
    
}

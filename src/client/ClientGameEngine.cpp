#include <client.hpp>
#include <iostream>
#include <thread>
#include <mutex>

#define REFRESH_ELEMENT 100

namespace client
{

void ClientGameEngine::handleInformation(int x, int y)
{
    std::cout << "User click on the Hex x=" << x << " & y=" << y << "\n";
}

void ClientGameEngine::startGameWindow(){
    clientGame.clientWindow([this](int x, int y) { handleInformation(x, y); });
}

void ClientGameEngine::renderGame() 
{
    std::thread t(&ClientGameEngine::startGameWindow, this);

    long lastUpdateTimer = clientGame.getCurrentTime(false);

    while(1){
        
        if (clientGame.getCurrentTime(false) - lastUpdateTimer > REFRESH_ELEMENT ){
            std::lock_guard<std::mutex> lock(clientGame.mutexGame);
            turn++;
            clientGame.loadElementTexture();
            lastUpdateTimer = clientGame.getCurrentTime(false);
        }

    }

    t.join();
}
    
}

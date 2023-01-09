#include <client.hpp>
#include <iostream>
#include <thread>
#include <sys/stat.h>

#define REFRESH_ELEMENT 1

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

namespace client
{

void ClientGameEngine::handleInformation(int x, int y)
{
    std::cout << "User click on the Hex x=" << x << " & y=" << y << "\n";
}

void ClientGameEngine::handleQuitMenu()
{
    std::lock_guard<std::mutex> lock(mutexRunningEngine);
    isWindowRunning[1] = !isWindowRunning[1];
    isWindowRunning[2] = !isWindowRunning[2];
}

void ClientGameEngine::startGameWindow(){
    clientGame.startGame(clientWindow, [this]() { handleQuitMenu(); }, [this](int x, int y) { handleInformation(x, y); });
}

void ClientGameEngine::startMenuWindow(){
    clientMenu.startMenu(clientWindow, [this]() { handleQuitMenu(); });
}

void ClientGameEngine::renderGame() 
{
    clientWindow = std::make_shared<sf::RenderWindow>();
    clientWindow->create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Civilization VII", sf::Style::Close);
    clientWindow->setPosition(sf::Vector2i(0, 0));

    while (true) {

        std::unique_lock<std::mutex> lockGlobalWhile(mutexRunningEngine);
        if (!isWindowRunning[0]) {
            lockGlobalWhile.unlock();
            break;
        }
        lockGlobalWhile.unlock();
        
        playGame();
        playMenu();
    }
}

void ClientGameEngine::playGame() 
{
    std::unique_lock<std::mutex> lockIf(mutexRunningEngine);
    if (isWindowRunning[2]) {
        lockIf.unlock();

        std::thread t(&ClientGameEngine::startGameWindow, this);
        t.detach();

        long lastUpdateTimer = clientGame.getCurrentTime();
        struct stat file_stat;
        std::string file_path = RESOURCES_PATH "/img/map/files.json";
        if (stat(file_path.c_str(), &file_stat) == -1) {
            std::cerr << "Erreur lors de l'obtention des informations sur le fichier " << file_path << '\n';
        }
        time_t last_modified = file_stat.st_mtime;

        while (true) {

            std::unique_lock<std::mutex> lockWhile(mutexRunningEngine);
            if (!isWindowRunning[2]) {
                lockWhile.unlock();
                break;
            }
            lockWhile.unlock();

            if (clientGame.getCurrentTime() - lastUpdateTimer > REFRESH_ELEMENT ){

                if (stat(file_path.c_str(), &file_stat) == -1) {
                    std::cerr << "Erreur lors de l'obtention des informations sur le fichier " << file_path << '\n';
                }

                time_t modified = file_stat.st_mtime;
                lastUpdateTimer = clientGame.getCurrentTime();

                if (modified != last_modified){
                    last_modified = modified;
                    clientGame.updateElementTexture();
                    turn++;
                }
            }
        }
    }
    else{
        lockIf.unlock();
    }
}

void ClientGameEngine::playMenu() 
{
    std::unique_lock<std::mutex> lockIf(mutexRunningEngine);
    if (isWindowRunning[1]) {
        lockIf.unlock();

        std::thread t(&ClientGameEngine::startMenuWindow, this);
        t.detach();

        while (true) {

            std::unique_lock<std::mutex> lockWhile(mutexRunningEngine);
            if (!isWindowRunning[1]) {
                lockWhile.unlock();
                break;
            }
            lockWhile.unlock();

            // Corps de la boucle while
        }
    }
    else{
        lockIf.unlock();
    }
}

}

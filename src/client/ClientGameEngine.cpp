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

void ClientGameEngine::handleQuitMenu(bool quitDef)
{
    std::lock_guard<std::mutex> lock(mutexRunningEngine);
    if (quitDef) runningWindow = 0;
    else runningWindow = runningWindow == 1 ? 2 : 1;
}

void ClientGameEngine::startGameWindow(){
    clientGame.startGame(clientWindow, [this](bool quitDef) { handleQuitMenu(quitDef); }, [this](int x, int y) { handleInformation(x, y); });
}

void ClientGameEngine::startMenuWindow(){
    clientMenu.startMenu(clientWindow, [this](bool quitDef) { handleQuitMenu(quitDef); });
}

void ClientGameEngine::renderGame() 
{
    clientWindow = std::make_shared<sf::RenderWindow>();
    clientWindow->create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Civilization VII", sf::Style::Close);
    clientWindow->setPosition(sf::Vector2i(0, 0));

    while (true) {

        std::unique_lock<std::mutex> lockGlobalWhile(mutexRunningEngine);
        if (!runningWindow) {
            lockGlobalWhile.unlock();
            clientWindow->close();
            return;
        }
        lockGlobalWhile.unlock();
        
        playGame();
        playMenu();
    }
}

void ClientGameEngine::playGame() 
{
    std::unique_lock<std::mutex> lockIf(mutexRunningEngine);
    if (runningWindow == 2) {
        lockIf.unlock();

        std::thread t(&ClientGameEngine::startGameWindow, this);

        long lastUpdateTimer = clientGame.getCurrentTime();
        struct stat file_stat;
        std::string file_path = RESOURCES_PATH "/img/map/files.json";
        if (stat(file_path.c_str(), &file_stat) == -1) {
            std::cerr << "Erreur lors de l'obtention des informations sur le fichier " << file_path << '\n';
        }
        time_t last_modified = file_stat.st_mtime;

        while (true) {

            std::unique_lock<std::mutex> lockWhile(mutexRunningEngine);
            if (runningWindow != 2) {
                lockWhile.unlock();
                t.join();
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
    if (runningWindow == 1) {
        lockIf.unlock();

        std::thread t(&ClientGameEngine::startMenuWindow, this);

        while (true) {

            std::unique_lock<std::mutex> lockWhile(mutexRunningEngine);
            if (runningWindow != 2) {
                lockWhile.unlock();
                t.join();
                break;
            }
            lockWhile.unlock();
        }
    }
    else{
        lockIf.unlock();
    }
}

}

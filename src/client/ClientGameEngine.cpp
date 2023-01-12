#include <client.hpp>
#include <iostream>
#include <sys/stat.h>

#define REFRESH_ELEMENT 1

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define MENU 1
#define GAME 2

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

namespace client
{

    ClientGameEngine::ClientGameEngine()
    {
        myself = std::make_shared<shared::Player>();
        myself->setUsername("PlayerTest");
    }

    void ClientGameEngine::connect(const std::string &serverAddress, int serverPort)
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(serverAddress), serverPort);

        boost::asio::ip::tcp::socket clientSocket(io_context);
        try
        {
            clientSocket.connect(endpoint);
        }
        catch (const boost::system::system_error &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            exit(1);
        }
        myself->setSocket(clientSocket);
        myself->state = shared::PlayerState::Connected;

        std::thread t(&ClientGameEngine::startReceiving, this);

        {
            std::lock_guard<std::mutex> lock(myself->socketWriteMutex);
            std::string msg = this->gameId + " " + myself->getName() + "\n";
            boost::asio::write(myself->getSocket(), boost::asio::buffer(msg));
        }

        t.detach();
    }

    void ClientGameEngine::startReceiving()
    {
        std::cout << "Starting receiving" << std::endl;

        while (myself->state == shared::PlayerState::Connected)
        {

            boost::asio::streambuf receiveBuffer;
            boost::system::error_code error;
            {
                std::lock_guard<std::mutex> lock(myself->socketReadMutex);
                boost::asio::read_until(myself->getSocket(), receiveBuffer, '\n', error);
            }

            if (error == boost::asio::error::operation_aborted)
            {
                myself->disconnectPlayer();
                continue;
            }
            if (receiveBuffer.size() > 0)
            {
                std::string messageReceived(
                    boost::asio::buffers_begin(receiveBuffer.data()),
                    boost::asio::buffers_end(receiveBuffer.data()));

                receiveBuffer.consume(receiveBuffer.size());

                if (messageReceived.find("response") == 0)
                {
                    registerServerAnswer(messageReceived);
                }
                else
                {
                    processServerRequest(messageReceived);
                }
            }
        }
    }

    void ClientGameEngine::registerServerAnswer(const std::string &response)
    {
        std::lock_guard<std::mutex> lock(myself->qAndA.sharedDataMutex);
        myself->qAndA.answer = response;
        myself->qAndA.answerReady = true;
        myself->qAndA.condition.notify_one();
    }

    void ClientGameEngine::processServerRequest(const std::string &request)
    {
        std::cout << "Received request: " << request << std::endl;
    }

    void ClientGameEngine::askServer()
    {
        myself->qAndA.answerReady = false;
        {
            std::lock_guard<std::mutex> lock(myself->socketWriteMutex);
            boost::asio::write(myself->getSocket(), boost::asio::buffer(myself->qAndA.question));
        }

        std::unique_lock<std::mutex> responseLock(myself->qAndA.sharedDataMutex);
        myself->qAndA.condition.wait(responseLock, [&]
                                     { return myself->qAndA.answerReady; });

        std::string response = myself->qAndA.answer;
        std::cout << "Received response: " << response << std::endl;
        myself->qAndA.answerReady = false;
    }

    void ClientGameEngine::handleInformation(int x, int y)
    {
        std::cout << "User click on the Hex x=" << x << " & y=" << y << "\n";
    }

    void ClientGameEngine::handleQuitMenu(bool quitDef)
    {
        std::lock_guard<std::mutex> lock(mutexRunningEngine);
        if (quitDef)
            runningWindow = 0;
        else
            runningWindow = runningWindow == MENU ? GAME : MENU;
    }

    void ClientGameEngine::startGameWindow()
    {
        clientGame.startGame(
            clientWindow, [this](bool quitDef)
            { handleQuitMenu(quitDef); },
            [this](int x, int y)
            { handleInformation(x, y); });
    }

    void ClientGameEngine::startMenuWindow()
    {
        clientMenu.startMenu(clientWindow, [this](bool quitDef)
                             { handleQuitMenu(quitDef); });
    }

    void ClientGameEngine::renderGame()
    {
        clientWindow = std::make_shared<sf::RenderWindow>();
        clientWindow->create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Civilization VII", sf::Style::Close);
        clientWindow->setPosition(sf::Vector2i(0, 0));

        while (true)
        {

            std::unique_lock<std::mutex> lockGlobalWhile(mutexRunningEngine);
            if (!runningWindow)
            {
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
        if (runningWindow == GAME)
        {
            lockIf.unlock();

            std::thread t(&ClientGameEngine::startGameWindow, this);

            long lastUpdateTimer = clientGame.getCurrentTime();
            struct stat file_stat;
            std::string file_path = RESOURCES_PATH "/img/map/files.json";
            if (stat(file_path.c_str(), &file_stat) == -1)
            {
                std::cerr << "Erreur lors de l'obtention des informations sur le fichier " << file_path << '\n';
            }
            time_t last_modified = file_stat.st_mtime;

            while (true)
            {

                std::unique_lock<std::mutex> lockWhile(mutexRunningEngine);
                if (runningWindow != GAME)
                {
                    lockWhile.unlock();
                    t.join();
                    break;
                }
                lockWhile.unlock();

                if (clientGame.getCurrentTime() - lastUpdateTimer > REFRESH_ELEMENT)
                {

                    if (stat(file_path.c_str(), &file_stat) == -1)
                    {
                        std::cerr << "Erreur lors de l'obtention des informations sur le fichier " << file_path << '\n';
                    }

                    time_t modified = file_stat.st_mtime;
                    lastUpdateTimer = clientGame.getCurrentTime();

                    if (modified != last_modified)
                    {
                        last_modified = modified;
                        clientGame.updateElementTexture();
                        turn++;
                    }
                }
            }
        }
        else
        {
            lockIf.unlock();
        }
    }

    void ClientGameEngine::playMenu()
    {
        std::unique_lock<std::mutex> lockIf(mutexRunningEngine);
        if (runningWindow == MENU)
        {
            lockIf.unlock();

            std::thread t(&ClientGameEngine::startMenuWindow, this);

            while (true)
            {

                std::unique_lock<std::mutex> lockWhile(mutexRunningEngine);
                if (runningWindow != MENU)
                {
                    lockWhile.unlock();
                    t.join();
                    break;
                }
                lockWhile.unlock();
            }
        }
        else
        {
            lockIf.unlock();
        }
    }

}
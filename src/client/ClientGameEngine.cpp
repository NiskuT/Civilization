#include <client.hpp>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

#define REFRESH_ELEMENT 1

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define MENU 1
#define GAME 2

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

using namespace client;

/*!
 * @brief Constructor
 *
 * Constructor of ClientGameEngine class
 */
ClientGameEngine::ClientGameEngine()
{
    clientMenu.gameEnginePtr = this;
    clientGame.gameEnginePtr = this;
    myself = std::make_shared<shared::Player>();
    myself->setUsername("PlayerTest");
}

/*!
 * @brief Quentin
 * @param serverAddress
 * @param serverPort
 */
bool ClientGameEngine::connect(const std::string &serverAddress, int serverPort)
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
        return false;
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

    return true;
}

/*!
 * @brief Quentin
 */
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
            processMessage(receiveBuffer);
            receiveBuffer.consume(receiveBuffer.size());

        }
    }
}

void ClientGameEngine::processMessage(boost::asio::streambuf& receiveBuffer)
{
    std::istream receiveStream(&receiveBuffer);
    std::string messageReceived;
    while(std::getline(receiveStream, messageReceived))
    {
        if (messageReceived.size() == 0)
        {
            continue;
        }
        else if (messageReceived.find("response") == 0)
        {
            registerServerAnswer(messageReceived);
        }
        else
        {
            processServerRequest(messageReceived);
        }
    }
}

/*!
 * @brief Quentin
 * @param
 */
void ClientGameEngine::registerServerAnswer(const std::string &response)
{
    std::lock_guard<std::mutex> lock(myself->qAndA.sharedDataMutex);
    myself->qAndA.answer = response;
    myself->qAndA.answerReady = true;
    myself->qAndA.condition.notify_one();
}

/*!
 * @brief Quentin
 * @param request
 */
void ClientGameEngine::processServerRequest(std::string request)
{
    if (request.find("chat") == 0)
    {
        request = request.substr(5);
        printChat(request);
    }
    else 
    {
        std::cout << "Received request: " << request << std::endl;
    }
}

void ClientGameEngine::printChat(const std::string &message)
{

    if (clientGame.chatBox != nullptr)
    {
        size_t firstSpace = message.find(" ");
        std::string chatTime = message.substr(0, firstSpace);
        size_t secondSpace = message.find(" ", firstSpace + 1);
        std::string chatUsername = message.substr(firstSpace + 1, secondSpace - firstSpace - 1);
        std::string chatMessage = message.substr(secondSpace + 1);

        clientGame.chatBox->updateChat(chatTime, chatUsername, chatMessage);
    }
}

/*!
 * @brief Quentin
 */
void ClientGameEngine::askServer()
{
    std::unique_lock<std::mutex> responseLock(myself->qAndA.sharedDataMutex);
    myself->qAndA.answerReady = false;
    {
        std::lock_guard<std::mutex> lock(myself->socketWriteMutex);
        boost::asio::write(myself->getSocket(), boost::asio::buffer(myself->qAndA.question));
    }

    myself->qAndA.condition.wait(responseLock, [&]
                                 { return myself->qAndA.answerReady; });
                                 
    std::string response = myself->qAndA.answer;
    myself->qAndA.answerReady = false;
}

/*!
 * @brief Print where the user click on the GameWindow
 * @param x position on x axis
 * @param y position on y axis
 */
void ClientGameEngine::handleInformation(int x, int y)
{
    std::cout << "User click on the Hex x=" << x << " & y=" << y << std::endl;
}

/*!
* @brief Print which priority card the user wants to play and its difficulty
* @param typePlayed type of the priority card played (economy, science, culture, ...)
* @param difficulty level of difficulty played (0 to 4 for the 5 fields) 
*/
void ClientGameEngine::handlePriorityCardPlay(std::string typePlayed, int difficulty)
{
    std::cout << "User wants to play " << typePlayed << " with a difficulty of " << difficulty << std::endl;
}

/*!
 * @brief Change the Window for nothing, Menu or Game
 * @param quitDef if quitDef is true, the game stop, else it change Menu to Game
 */
void ClientGameEngine::handleQuitMenu(bool quitDef)
{
    if (quitDef)
    {
        runningWindow.store(0);
    }
    else
    {
        runningWindow.store(runningWindow == MENU ? GAME : MENU);
    }
}

/*!
 * @brief A player is connecting to a Game
 * @param id game ID, = new for a new Game
 * @param username player username
 * @param server serveur adresse
 * @param port port number
 */
bool ClientGameEngine::tryConnection(std::string id, std::string username, std::string server, std::string port)
{
    myself->setUsername(username);

    int portNumber;
    std::stringstream sStream(port);
    if ((sStream >> portNumber).fail())
    {
        return false;
    }

    gameId = id;

    return connect(server, portNumber);
}

/*!
 * @brief Start the loop that manage the GameWindow
 */
void ClientGameEngine::startGameWindow()
{
    clientGame.startGame();
}

/*!
 * @brief Start the loop that manage the MenuWindow
 */
void ClientGameEngine::startMenuWindow()
{
    clientMenu.startMenu();
}

/*!
 * @brief Loop that manage the entire Engine
 */
void ClientGameEngine::renderGame()
{
    clientWindow = std::make_shared<sf::RenderWindow>();
    clientWindow->create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH),
                         "Civilization VII",
                         sf::Style::Close);

    clientWindow->setPosition(sf::Vector2i(0, 0));

    while (runningWindow.load())
    {
        if (runningWindow.load() == GAME)
        {
            playGame();
        }
        else if (runningWindow.load() == MENU)
        {
            playMenu();
        }
    }
    clientWindow->close();
}

/*!
 * @brief Loop that is used when the client is in GameMode
 */
void ClientGameEngine::playGame()
{
    std::thread t(&ClientGameEngine::startGameWindow, this);

    long lastUpdateTimer = clientGame.getCurrentTime();
    struct stat file_stat;
    std::string file_path = RESOURCES_PATH "/map/files.json";

    if (stat(file_path.c_str(), &file_stat) == -1)
    {
        std::cerr << "Erreur lors de l'obtention des informations sur le fichier " << file_path << '\n';
    }

    time_t last_modified = file_stat.st_mtime;

    while (runningWindow.load() == GAME)
    {
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
    t.join();
}

/*!
 * @brief Loop that is used when the client is in MenuMode
 */
void ClientGameEngine::playMenu()
{
    std::thread t(&ClientGameEngine::startMenuWindow, this);
    t.join();
}

/*!
 * @brief Detect an intersection between a point and a rect
 * @param point the point
 * @param rectangle the rectangle
 */
bool ClientGameEngine::intersectPointRect(sf::Vector2i point, sf::FloatRect rectangle)
{
    return (point.x >= rectangle.left &&
            point.x <= rectangle.left + rectangle.width &&
            point.y >= rectangle.top &&
            point.y <= rectangle.top + rectangle.height);
}

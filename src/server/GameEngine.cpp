#include <server.hpp>
#include <algorithm>
#include <random>
#include <iomanip>
#include <iostream>

#define MAX_PLAYERS 4

using namespace server;

/*!
 * @brief Quentin
 */
GameEngine::GameEngine(std::vector<std::shared_ptr<GameEngine>> &games, std::shared_ptr<shared::Player> player)
{
    gameId = GameEngine::generateRandomId(games);
    players.push_back(player);
}

/*!
 * @brief Quentin
 * @param player
 */
bool GameEngine::addPlayer(std::shared_ptr<shared::Player> player)
{
    if (isPublic && (players.size() < MAX_PLAYERS))
    {
        players.push_back(player);
        gameMap = std::make_unique<shared::Map>();
    }
    else
    {
        return false;
    }

    if (players.size() == MAX_PLAYERS)
    {
        state = GameState::Running;
    }
    return true;
}

/*!
 * @brief Quentin
 */
std::vector<std::shared_ptr<shared::Player>> &GameEngine::getPlayers()
{
    return players;
}

/*!
 * @brief Quentin
 * @param games
 */
std::string GameEngine::generateRandomId(const std::vector<std::shared_ptr<GameEngine>> &games)
{
    static const std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<std::string::size_type> distribution(0, alphabet.size() - 1);

    std::string id;
    do
    {
        id.resize(6);
        std::generate(id.begin(), id.end(), [&]
                      { return alphabet[distribution(generator)]; });
    } while (std::any_of(games.begin(), games.end(), [&](const auto &game)
                         { return game->getId() == id; }));
    return id;
}

/*!
 * @brief Quentin
 */
std::string GameEngine::getId()
{
    return gameId;
}

/*!
 * @brief Quentin
 * @param requestString
 * @param player
 */
void GameEngine::processClientRequest(std::string requestString, std::shared_ptr<shared::Player> player)
{
    std::vector<std::string> requestComponents = splitString(requestString, ' ');
    if (requestComponents.size() < 1)
    {
        return;
    }

    std::string command = requestComponents[0];

    std::string response = "response error: invalid command\n";
    if (command.find("getstate") == 0)
    {
        response = "response" + player->getName() + " is connected\n";
    }
    else if (command.find("chat") == 0)
    {
        requestString = requestString.substr(5);
        std::string message = "chat ";
        message += getTime() + " ";
        message += player->getName() + " " + requestString + "\n";
        sendToEveryone(message);
        response = "response ok\n";
    }
    else if (command.find("getmap") == 0)
    {
        if (gameMap == nullptr)
        {
            response = "error: map not initialized\n";
        }
        else
        {
            binary.castToBinary(*gameMap, response);
            binary.send(player, response);
            return;
        }
    }
    else if (command.find("setmapparam") == 0)
    {
        if (requestComponents.size() == 3)
        {
            std::string param = requestComponents[1];
            std::string value = requestComponents[2];
            response = setMapParam(param, value) ? "response ok\n" : "response error: invalid parameter\n";
        }
    }

    std::lock_guard<std::mutex> lock(player->socketWriteMutex);
    boost::asio::write(player->getSocket(), boost::asio::buffer(response));
}

/*!
 * @brief Quentin
 * @param param
 * @param value
 */
bool GameEngine::setMapParam(std::string &param, std::string &value)
{
    int paramValue;
    try
    {
        paramValue = std::stoi(value);
    }
    catch (std::exception& e)
    {
        return false;
    }
    if (param.find("width") == 0)
    {
        gameMap->setMapWidth(paramValue);
    }
    else if (param.find("height") == 0)
    {
        gameMap->setMapHeight(paramValue);
    }
    else if (param.find("generate") == 0)
    {
        gameMap->generateRandomMap(paramValue);
    }
    else
    {
        return false;
    }
    return true;
}

/*!
 * @brief Quentin
 * @param str
 * @param delimiter
 */
std::vector<std::string> GameEngine::splitString(std::string str, char delimiter)
{
    std::vector<std::string> components;
    std::stringstream ss(str);
    std::string component;
    while (std::getline(ss, component, delimiter))
    {
        components.push_back(component);
    }
    return components;
}

/*!
 * @brief Quentin
 * @param player
 */
void GameEngine::askClient(std::shared_ptr<shared::Player> player)
{
    std::unique_lock<std::mutex> responseLock(player->qAndA.sharedDataMutex);
    player->qAndA.answerReady = false;
    {
        std::lock_guard<std::mutex> lock(player->socketWriteMutex);
        boost::asio::write(player->getSocket(), boost::asio::buffer(player->qAndA.question));
    }

    player->qAndA.condition.wait(responseLock, [&]
                                 { return player->qAndA.answerReady; });

    std::string response = player->qAndA.answer;
    player->qAndA.answerReady = false;
}

/*!
 * @brief Quentin
 * @param message
 */
void GameEngine::sendToEveryone(std::string message)
{
    std::cout << message;
    for (auto &player : players)
    {
        if (player->connectedToSocket.load())
        {
            std::lock_guard<std::mutex> lock(player->socketWriteMutex);
            boost::asio::write(player->getSocket(), boost::asio::buffer(message));
        }
    }
}

/*! 
 * @brief This function return the time under a string format
 * For example, if the time is 1:34pm, the function will return "13:34"
 * @return std::string
 */
std::string GameEngine::getTime()
{
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%H:%M");
    return ss.str();
}
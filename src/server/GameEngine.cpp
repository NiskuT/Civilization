#include <server.hpp>
#include <algorithm>
#include <random>
#include <iomanip>
#include <iostream>

#define MAX_PLAYERS 2

using namespace server;

GameEngine::GameEngine(std::vector<std::shared_ptr<GameEngine>> &games, std::shared_ptr<shared::Player> player)
{
    gameId = GameEngine::generateRandomId(games);
    gameMap = std::make_shared<shared::Map>();
    players.push_back(player);
}

bool GameEngine::addPlayer(std::shared_ptr<shared::Player> player)
{
    if (isPublic && (players.size() < MAX_PLAYERS))
    {
        players.push_back(player);
    }
    else
    {
        return false;
    }

    if (players.size() == MAX_PLAYERS)
    {
        std::cout << "Tous les joueurs sont connectés\n"
                  << std::endl;
        std::thread gameThread(&GameEngine::runGame, this);
        gameThread.detach();
    }
    return true;
}

void GameEngine::askClientToPlayARule(std::shared_ptr<shared::Player> player, shared::RuleArgsStruct &ruleArgs)
{

    shared::Rules rules;
    do
    {
        std::unique_lock<std::mutex> lock(player->qAndA.sharedDataMutex);
        player->qAndA.question = "playturn\n";
        lock.unlock();
        askClient(player);

        lock.lock();
        binary.castToObject(player->qAndA.answer, ruleArgs);
        lock.unlock();

        ruleArgs.gameMap = this->gameMap;
        ruleArgs.currentPlayer = player;

    } while (!(rules.runTheRule(ruleArgs)));
}

void GameEngine::runGame() // rename rungame
{
    std::cout << "start game" << std::endl;
    std::cout << "map initialized" << std::endl;
    std::string message = "chat ";
    message += getTime() + " ";
    message += "Game started\n";
    sendToEveryone(message);
    while (true) // TODO: add condition to stop the game (victory of a player)
    {
        for (auto player : players)
        {
            std::cout << "start of turn of player: " << player->getName() << std::endl;
            shared::RuleArgsStruct ruleArgs;
            askClientToPlayARule(player, ruleArgs);

            ruleArgs.playerName = player->getName();

            std::string struc;
            binary.castToBinary(ruleArgs, struc);
            sendToEveryone(struc, true);
        }
    }
}

std::vector<std::shared_ptr<shared::Player>> &GameEngine::getPlayers()
{
    return players;
}

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

std::string GameEngine::getId()
{
    return gameId;
}

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

bool GameEngine::setMapParam(std::string &param, std::string &value)
{
    int paramValue;
    try
    {
        paramValue = std::stoi(value);
    }
    catch (std::exception &e)
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

void GameEngine::sendToEveryone(std::string message, bool isBinary)
{
    for (auto &player : players)
    {
        if (!isBinary && player->connectedToSocket.load())
        {
            std::lock_guard<std::mutex> lock(player->socketWriteMutex);
            boost::asio::write(player->getSocket(), boost::asio::buffer(message));
        }
        else if (player->connectedToSocket.load())
        {
            binary.send(player, message, false);
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
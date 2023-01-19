#include <shared.hpp>

using namespace shared;

Player::Player()
{
    connectedToSocket.store(false);
    for (int i = 1; i < 4; i++)
    {
        this->ressources[i] = 0;
        this->wonderRessources[i] = 0;
    }
}

void Player::setUsername(std::string username)
{
    this->username = username;
}

void Player::setSocket(boost::asio::ip::tcp::socket &clientSocket)
{
    if (connectedToSocket.load())
    {
        disconnectPlayer();
    }
    connectedToSocket.store(true);
    this->playerSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(clientSocket));
}

std::string Player::getName()
{
    return this->username;
}

bool Player::operator==(Player &otherPlayer)
{
    return (this->getName() == otherPlayer.getName() &&
            connectedToSocket.load() == false &&
            otherPlayer.connectedToSocket.load() == false);
}

boost::asio::ip::tcp::socket &Player::getSocket()
{
    return *(this->playerSocket.get());
}

void Player::disconnectPlayer()
{
    connectedToSocket.store(false);
    std::lock_guard<std::mutex> socketLock(socketReadMutex);
    std::lock_guard<std::mutex> socketLock2(socketWriteMutex);
    if (playerSocket && playerSocket->is_open())
    {
        playerSocket->close();
    }
    playerSocket.reset();
}

#include <shared.hpp>

namespace shared
{

    Player::Player()
    {
        this->state = PlayerState::WaitingForGame;
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
        if (state == PlayerState::Connected)
        {
            disconnectPlayer();
        }
        state = shared::PlayerState::Connected;
        this->playerSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(clientSocket));
    }

    std::string Player::getName()
    {
        return this->username;
    }

    bool Player::operator==(Player &otherPlayer)
    {
        return (this->getName() == otherPlayer.getName() && this->state == PlayerState::WaitingForGame && otherPlayer.state == PlayerState::Disconnected) || (this->getName() == otherPlayer.getName() && this->state == PlayerState::Disconnected && otherPlayer.state == PlayerState::WaitingForGame);
    }

    boost::asio::ip::tcp::socket &Player::getSocket()
    {
        return *(this->playerSocket.get());
    }

    void Player::disconnectPlayer()
    {
        state = shared::PlayerState::Disconnected;
        std::lock_guard<std::mutex> socketLock(socketReadMutex);
        std::lock_guard<std::mutex> socketLock2(socketWriteMutex);
        if (playerSocket && playerSocket->is_open())
        {
            playerSocket->close();
        }
        playerSocket.reset();
    }

}
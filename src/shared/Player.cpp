#include <shared.hpp>

namespace shared
{

    Player::Player(boost::asio::ip::tcp::socket &clientSocket)
    {

        this->playerSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(clientSocket));
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
        playerSocket->close();
        playerSocket.reset();
    }

    void Player::reconnect (boost::asio::ip::tcp::socket& clientSocket)
    {
        state = shared::PlayerState::Connected;
        this->playerSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(clientSocket));
    }
}
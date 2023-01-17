#include <shared.hpp>

using namespace shared;

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

std::array<int, 3> Player::incrementTechWheel(unsigned toIncrement)
{
    std::array<int, 3> result;
    for (unsigned i = 0; i < toIncrement; i++)
    {
        this->techLevel += 1;
        if (this->techLevel == 3 || this->techLevel == 6)
        {
            result[0]++;
        }
        else if (this->techLevel == 10 || this->techLevel == 14)
        {
            result[1]++;
        }
        else if (this->techLevel == 19 || this->techLevel == 24)
        {
            result[2]++;
        }
    }
    return result;
}

unsigned Player::getLevelOfCard(CardsEnum cardType)
{
    for (auto card : this->listOfPriorityCard)
    {
        if (card->getType() == cardType)
        {
            return card->getLevel();
        }
    }
}

void Player::upgradeCard(CardsEnum cardType)
{
    for (auto card : this->listOfPriorityCard)
    {
        if (card->getType() == cardType)
        {
            card->addLevel();
        }
    }
}

void Player::addBox(CardsEnum cardType, unsigned numberOfBoxToAdd)
{
    for (auto card : this->listOfPriorityCard)
    {
        if (card->getType() == cardType)
        {
            card->addBox(numberOfBoxToAdd);
        }
    }
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

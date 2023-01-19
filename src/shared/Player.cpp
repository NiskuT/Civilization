#include <shared.hpp>

#define TECH_LEVEL_1 3
#define TECH_LEVEL_2 6
#define TECH_LEVEL_3 10
#define TECH_LEVEL_4 14
#define TECH_LEVEL_5 19
#define TECH_LEVEL_END 24
#define TECH_LEVEL_RESET 17

using namespace shared;

Player::Player()
{
    connectedToSocket.store(false);
    std::fill(resources.begin(), resources.end(), 0);
    this->techLevel = 0;
    this->listOfPriorityCard[0] = std::make_shared<Card>(CardsEnum::military);
    this->listOfPriorityCard[1] = std::make_shared<Card>(CardsEnum::economy);
    this->listOfPriorityCard[2] = std::make_shared<Card>(CardsEnum::science);
    this->listOfPriorityCard[3] = std::make_shared<Card>(CardsEnum::industry);
    this->listOfPriorityCard[4] = std::make_shared<Card>(CardsEnum::culture);
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

std::array<int, 3> Player::incrementTechWheel(unsigned toIncrement)
{
    std::array<int, 3> result;
    std::fill(result.begin(), result.end(), 0);
    for (unsigned i = 0; i < toIncrement; i++)
    {
        this->techLevel += 1;
        if (this->techLevel == TECH_LEVEL_1 || this->techLevel == TECH_LEVEL_2)
        {
            result[0]++;
        }
        else if (this->techLevel == TECH_LEVEL_3 || this->techLevel == TECH_LEVEL_4)
        {
            result[1]++;
        }
        else if (this->techLevel == TECH_LEVEL_5 || this->techLevel == TECH_LEVEL_END)
        {
            result[2]++;
        }
        if(this->techLevel == TECH_LEVEL_END)
        {
            this->techLevel = TECH_LEVEL_RESET;
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
    return 0;
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

std::vector<std::shared_ptr<Caravan>> Player::getCaravans()
{
    return this->listOfCaravan;
}

unsigned Player::getNumberOfBox(CardsEnum cardType)
{
    for (auto card : this->listOfPriorityCard)
    {
        if (card->getType() == cardType)
        {
            return card->getNumberOfBox();
        }
    }
    return 0;
}

void Player::addCaravan(std::shared_ptr<Caravan> newCaravan)
{
    this->listOfCaravan.push_back(newCaravan);
}

bool Player::haveResource(ResourceEnum resourceType)
{
    return this->resources[(int)resourceType] > 0;
}

void Player::addControlPawn(std::shared_ptr<ControlPawn> newControlPawn)
{
    this->listOfControlPawn.push_back(newControlPawn);
}

void Player::removeControlPawn(std::shared_ptr<ControlPawn> controlPawnToRemove)
{
    for (auto controlPawn : this->listOfControlPawn)
    {
        if (controlPawn == controlPawnToRemove)
        {
            this->listOfControlPawn.erase(std::remove(this->listOfControlPawn.begin(), this->listOfControlPawn.end(), controlPawnToRemove), this->listOfControlPawn.end());
        }
    }
}

void Player::addResource(ResourceEnum resourceType)
{
    this->resources[(int)resourceType] += 1;
}

unsigned Player::getTechLevel()
{
    return this->techLevel;
}

unsigned Player::getNumberOfResource(ResourceEnum resourceType)
{
    return this->resources[(int)resourceType];
}

void Player::deleteBox(CardsEnum cardType, unsigned numberOfBoxToDelete)
{
    for (auto card : this->listOfPriorityCard)
    {
        if (card->getType() == cardType)
        {
            card->deleteBox(numberOfBoxToDelete);
        }
    }
}

std::vector<std::shared_ptr<City>> Player::getCityList()
{
    return this->listOfCity;
}

void Player::addCity(std::shared_ptr<City> newCity)
{
    this->listOfCity.push_back(newCity);
}

std::vector<std::shared_ptr<ControlPawn>> Player::getControlPawns()
{
    return this->listOfControlPawn;
}

void Player::setDificultyOfCard(CardsEnum cardType, unsigned newDificulty)
{
    for (auto card : this->listOfPriorityCard)
    {
        if (card->getType() == cardType)
        {
            card->setDificulty(newDificulty);
        }
    }
}

unsigned Player::getDificultyOfCard(CardsEnum cardType)
{
    for (auto card : this->listOfPriorityCard)
    {
        if (card->getType() == cardType)
        {
            return card->getDificulty();
        }
    }
    return 0;
}

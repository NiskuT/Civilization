#include <shared.hpp>

using namespace shared;

Player::Player(std::string username)
{
    this->username = username;
    for (int i = 1; i < 4; i++)
    {
        this->resource[i] = 0;
        this->wonderRessources[i] = 0;
    }
}

std::string Player::getName()
{
    return this->username;
}

std::array<int, 3> Player::incrementTechWheel(unsigned toIncrement)
{
    std::array<int, 3> result;
    for (int i = 0; i < toIncrement; i++)
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

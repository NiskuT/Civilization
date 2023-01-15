#include <shared.hpp>

namespace shared
{

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

}
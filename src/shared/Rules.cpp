#include <client.hpp>
#include <shared.hpp>
#include <iostream>
#include <array>

#define RULESLENGTH 16

////// faire une enum classe pour les cartes, chaque carte a un numero

namespace shared
{
    /**
     * @file Rules.cpp
     * @fn Rules::Rules(unsigned ruleId)
     * @brief constructor of Rules, define the ruleId of created rule.
     */
    Rules::Rules(unsigned ruleId)
    {
        if (ruleId > RULESLENGTH)
        {
            printf("out ");
            exit(1);
        }
        this->ruleId = ruleId;
    }

    void Rules::runTheRule()
    {
        unsigned cardLevel;
        switch (this->ruleId)
        {
        // Economy cards
        case 0:
            cardLevel = this->player->getLevelOfCard(CardsEnum::economy); // DEFINE THIS GETTER IN PLAYER AND USE #DEFINE OR ENUM FOR ECONOMY
            switch (cardLevel)
            {
            case 1:
                moveCaravan(3, cardLevel, false, false); // one caravane can move up to 3 steps
                break;
            case 2:
                moveCaravan(4, cardLevel, true, false); // one caravane can move up to 3
                moveCaravan(4, cardLevel, true, false);
                break;
            case 3:
                moveCaravan(6, cardLevel, false, true); // one caravane can move up to 3
                moveCaravan(6, cardLevel, false, true);
                exchangeRessourceWithOtherPlayer();
                break;
            case 4:
                moveCaravan(6, cardLevel, false, true); // one caravane can move up to 3
                moveCaravan(6, cardLevel, false, true);
                moveCaravan(6, cardLevel, false, true);
                playAnotherCard();
                break;
            default:
                std::cout << "invalid card level" << std::endl;
                exit(1);
            }

            break;
        // Science cards
        case 1:
            cardLevel = this->player->getLevelOfCard(CardsEnum::science);
            incrementTechWheel(cardLevel);
            switch (cardLevel)
            {
            case 1:
                // do nothing special
                break;
            case 2:
                getABox();
                break;
            case 3:
                getAnUnpossesedRessource();
                break;
            case 4:
                nuke();
                break;
            default:
                std::cout << "invalid card level" << std::endl;
                exit(1);
            }
            break;
        default:
            std::cout << "This rule doesn't have utility" << std::endl;
        }
    }

    /**
     * @file Rules.cpp
     * @fn void Rules::moveCaravan()
     * @brief This function aim to allow the player to move one of his caravans
     */
    void Rules::moveCaravan(int distance, int maxLevelReachable, bool caravanKiller, bool canMoveOnWater)
    {
        // boxes can change the distance of caravans
        int numberOfBoxUsed = 0;
        if (this->player->getNumberOfBox(CardsEnum::economy) != 0)
            numberOfBoxUsed = this->clientGameEngine->askForUsingBox();
            // possible ?????

        std::array<int, 10> caravanMovementPath; // case parcourue x et y et a la fin, le nombre de cases parcourues
        do
        {
            // ask the player to move a caravan and get the different hexagons crossed.
            caravanMovementPath = this->clientGameEngine->moveObject(MovableObjectEnum::caravan, maxLevelReachable, caravanKiller, canMoveOnWater); //  A FUNCTION NEED TO BE IMPLEMENTED TO LET THE PLAYER CHOOSE THE CARAVAN HE WANT TO MOVE
            // return the path of the caravan. start from the caravan position and end with the destination
        } while (!this->verifyIfPathIsValid(caravanMovementPath, maxLevelReachable /*+ potentialBoost*/)); // define potential boost (special action that augment the range of a caravan)

        this->player->getCaravanAt(caravanMovementPath[0], caravanMovementPath[1])->move(caravanMovementPath);
        // add server move
    }

    /**
     * @file Rules.cpp
     * @fn bool Rules::verifyIfPathIsValid()
     * @brief This function verify if a caravan can travel a certain path depending on it's level.
     */
    bool Rules::verifyIfPathIsValid(std::array<int, 10> caravanMovementPath, int maxLevelReachable)
    {
        int i;
        for (i = 0; i < caravanMovementPath.size() / 2; i++)
        {
            if (this->clientGameEngine->map[caravanMovementPath[i * 2], caravanMovementPath[i * 2 + 1]] > maxLevelReachable)
            {
                std::cout << "Invalid movement" << std::endl;
                return false;
            }
        }
        return true;
    }

    void Rules::exchangeRessourceWithOtherPlayer()
    {
        std::array<PlayerResource,2> resourceToExchange;
        do
        {
            resourceToExchange = this->clientGameEngine->exchangeResource();                                                                                            // ask the player to choose cards to exchange
        } while (resourceToExchange[0].player->haveResource(*(resourceToExchange[0].resource)) && resourceToExchange[1].player->haveResource(*(resourceToExchange[1].resource))); // il faut verifier que les joueurs possedes les cartes

        //TODO 
    }

    // void Rules::playAnotherCard()
    // {
    //     do
    //     {
    //         Card *cardPlayed = this->clientGameEngine->chooseCard(cardsEnum::science, cardsEnum::army, cardsEnum::culture, cardsEnum::industry); // chooseCard function must be used with cards that can be played
    //     } while (cardPlayed.getId() == cardsEnum::economy);
    //     this->gameEngine->playCard(cardPlayed);
    // }

    // void Rules::incrementTechWheel(unsigned cardLevel)
    // {
    //     int cardAmelioration[3] = this->player->incrementTechWheel(cardLevel); // incrementTechWheel function return tab[3] of int.
    //     // tab[0]: number of cards that can be upgrade from level 1 to 2...
    //     // this function must reset counter to 17 while 25 is obtained
    //     for (int i = 0; i < 3; i++)
    //     {
    //         for (int numberOfCards = 0; numberOfCards < cardAmelioration[i]; i++)
    //         {
    //             unsigned numberOfTheCard; // variable used to verify that the level of card is valid (member of enum class "cardsEnum")
    //             do
    //             {
    //                 numberOfTheCard = this->clientGameEngine->askForUpgrade(i); // tell the client the new level
    //             } while (this->player->getCardlevel[numberOfTheCard] != cardLevel - 1);
    //             this->player->upgradeCard(numberOfTheCard);
    //         }
    //     }
    // }

    // void Rules::getABox()
    // {
    //     unsigned cardNumber = clientGameEngine.askForNewBoxPosition(); // ask the player where he want to place the box
    //     this->player->getCard(cardNumber).addBox();
    // }

    // void Rules::getAnUnpossesedRessource()
    // {
    //     unsigned resource;
    //     do
    //     {
    //         resource = clientGameEngine.askForANewResource();       // Resource is the interger associated to the Resource.
    //     } while (this->player->getNumberOfResource(Resource) != 0); // if the Resource is not already possessed

    //     this->player->addResource(resource); // add the Resource to the list
    // }

    // void Rules::nuke()
    // {
    //     int position[2] = this->clientGameEngine->askForNukePosition();
    //     for (int x = -1; x < 2; x += 2)
    //     {
    //         for (int y = 0; y < 2; y++) // for the 2 up and 2 down cases
    //         {
    //             if (map[position[0] + x][position[1] + !(position[0] % 2) + y /*the next cases depends on the line number*/] == Controlpawn)
    //             {
    //                 pawn.getPawn(position[0] + x, position[1] + !(position[0] % 2) + y).downgrade(); // get the control pawn and kill or downdrade it
    //             }
    //         }
    //     }
    //     for (int y = -1; y < 2; y++) // for the 2 cases at the right and left
    //     {
    //         if (map[position[0]][position[1] + y] == controlpawn) // check if there is a control pawn on the case
    //         {
    //             pawn.getPawn(position[0], position[1] + y).downgrade(); // get the control pawn and kill or downdrade it
    //         }
    //     }
    // }

    // void Rules::reinforcePawn()
    // {
    //     int position[2];
    //     do
    //     {
    //         position[2] = clientGameEngine.askForAPawnToFeinforce();
    //     } while (this->player->haveControlPawnIn(position[0], position[1]) == false);
    //     this->player->getpawn(position[0], position[1]);
    // }

    // void Rules::attack()
    // {
    // }

}

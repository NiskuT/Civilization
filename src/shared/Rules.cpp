#include <client.hpp>
#include <shared.hpp>
#include <iostream>
#include <array>

#define RULESLENGTH 16
#define CARAVAN_STEPS_AT_LEVEL_1 3
#define CARAVAN_STEPS_AT_LEVEL_2 4
#define CARAVAN_STEPS_AT_LEVEL_3 6
#define CARAVAN_STEPS_AT_LEVEL_4 6

namespace shared
{
    /**
     * @file Rules.cpp
     * @fn Rules::Rules(unsigned ruleId)
     * @brief constructor of Rules, define the ruleId of created rule.
     */
    Rules::Rules(CardsEnum ruleId)
    {
        this->ruleId = ruleId;
    }

    /**
     * @file Rules.cpp
     * @fn void Rules::runTheRule(unsigned numberOfBoxUsed)
     * @brief This function aim to run the rule of the card
     */
    void Rules::runTheRule(ArgsOfRule *args)
    {
        switch (this->ruleId)
        {
        case CardsEnum::economy:
            playEconomyCard(args);
            break;

        case CardsEnum::science:
            playScienceCard(args);
            break;

        default:
            std::cout << "This rule doesn't have utility" << std::endl;
        }
    }

    /**
     * @file Rules.cpp
     * @fn void Rules::playEconomyCard()
     * @brief This function aim to run the rule of the economy card
     */
    void Rules::playEconomyCard(ArgsOfRule *args)
    {
        // retrieve the arguments
        std::vector<int[2]> *caravanMovementPath = args->economyCardRuleArgs.caravanMovementPath;
        Map *map = args->economyCardRuleArgs.mapOfTheGame;
        unsigned numberOfBoxUsed = args->economyCardRuleArgs.numberOfBoxUsed;

        // check if the player has enough box to play this card
        if (numberOfBoxUsed > this->player->getNumberOfBox(CardsEnum::economy))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        unsigned cardLevel = this->player->getLevelOfCard(CardsEnum::economy);

        std::vector<Barbarian *> barbarianList = checkIfBarbarianIsOnThePath(caravanMovementPath, map);
        if (barbarianList.size() > 0 && cardLevel != 2)
        {
            std::cout << "You can't kill a barbarian with this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        bool isWaterOnThePath = checkIfWaterIsOnThePath(caravanMovementPath, map);
        if (isWaterOnThePath && cardLevel < 3)
        {
            std::cout << "You can't move on water with this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        unsigned maxLevelReachable = cardLevel; // TODO: check if the player can reach a higher level depending on his caracteristics

        switch (cardLevel)
        {
        case 1:
            moveCaravan(CARAVAN_STEPS_AT_LEVEL_1 + numberOfBoxUsed, maxLevelReachable);
            break;
        case 2:
            if (barbarianList.size() > 0)
            {
                for (int i = 0; i < barbarianList.size(); i++)
                {
                    barbarianList.at(i)->kill();
                    this->player->addBox(CardsEnum::economy); // TODO: allow to add box on other card
                }
            }
            moveCaravan(CARAVAN_STEPS_AT_LEVEL_2 + numberOfBoxUsed, maxLevelReachable);
            moveCaravan(CARAVAN_STEPS_AT_LEVEL_2 + numberOfBoxUsed, maxLevelReachable);
            break;
        case 3:
            moveCaravan(CARAVAN_STEPS_AT_LEVEL_3 + numberOfBoxUsed, maxLevelReachable);
            moveCaravan(CARAVAN_STEPS_AT_LEVEL_3 + numberOfBoxUsed, maxLevelReachable);
            exchangeRessourceWithOtherPlayer();
            break;
        case 4:
            moveCaravan(CARAVAN_STEPS_AT_LEVEL_4 + numberOfBoxUsed, maxLevelReachable);
            moveCaravan(CARAVAN_STEPS_AT_LEVEL_4 + numberOfBoxUsed, maxLevelReachable);
            moveCaravan(CARAVAN_STEPS_AT_LEVEL_4 + numberOfBoxUsed, maxLevelReachable);
            playAnotherCard();
            break;
        default:
            std::cout << "invalid card level" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    /**
     * @file Rules.cpp
     * @fn void Rules::checkIfBarbarianIsOnThePath(std::vector<int[2]> *caravanMovementPath)
     * @brief This function aim to check if a barbarian is on the path of the caravan
     */
    std::vector<Barbarian *> Rules::checkIfBarbarianIsOnThePath(std::vector<int[2]> *caravanMovementPath, shared::Map *map)
    {
        std::vector<Barbarian *> barbarianList;
        Barbarian *barbarian;
        for (int i = 0; i < caravanMovementPath->size(); i++)
        {
            unsigned x = caravanMovementPath->at(i)[0];
            unsigned y = caravanMovementPath->at(i)[1];
            barbarian = map->operator()(x, y)->getBarbarian();
            if ( barbarian != nullptr)
            {
                barbarianList.push_back(barbarian);
            }
        }
        return barbarianList;
    }

    /**
     * @file Rules.cpp
     * @fn void Rules::checkIfWaterIsOnThePath(std::vector<int[2]> *caravanMovementPath)
     * @brief This function aim to check if water is on the path of the caravan
    */
    bool Rules::checkIfWaterIsOnThePath(std::vector<int[2]> *caravanMovementPath, shared::Map *map)
    {
        for (int i = 0; i < caravanMovementPath->size(); i++)
        {
            int x = caravanMovementPath->at(i)[0];
            int y = caravanMovementPath->at(i)[1];
            if (map->operator()(x, y)->getFieldLevel() == FieldLevel::Water)
            {
                return true;
            }
        }
        return false;
    }

    // /**
    //  * @file Rules.cpp
    //  * @fn void Rules::playScienceCard()
    //  * @brief This function aim to run the rule of the science card
    //  */
    // void Rules::playScienceCard(ArgsOfRule *args)
    // {
    //     if (numberOfBoxUsed > this->player->getNumberOfBox(CardsEnum::science))
    //     {
    //         std::cout << "You don't have enough box to play this card" << std::endl;
    //         exit(EXIT_FAILURE);
    //     }

    //     unsigned cardLevel = this->player->getLevelOfCard(CardsEnum::science);
    //     switch (cardLevel)
    //     {
    //     case 1:
    //         // do nothing special
    //         break;
    //     case 2:
    //         getABox();
    //         break;
    //     case 3:
    //         getAnUnpossesedRessource();
    //         break;
    //     case 4:
    //         nuke();
    //         break;
    //     default:
    //         std::cout << "invalid card level" << std::endl;
    //         exit(1);
    //     }
    // }

    // /**
    //  * @file Rules.cpp
    //  * @fn void Rules::moveCaravan()
    //  * @brief This function aim to allow the player to move one of his caravans
    //  */
    // void Rules::moveCaravan(int distance, int maxLevelReachable, bool caravanKiller, bool canMoveOnWater)
    // {
    //     std::vector<int[2]> caravanMovementPath; /**List of coordinates of the path*/
    //     do
    //     {
    //         caravanMovementPath = this->clientGameEngine->moveObject(MovableObjectEnum::caravan, maxLevelReachable, caravanKiller, canMoveOnWater);
    //         /*
    //         Create a function in the client game engine that ask the player to move a MoveableObjectEnum
    //         This function must be named "moveObject"
    //         This function must return a vector of int[2] that contains the coordinates of the path (std::vector<int[2]>)
    //         */
    //     } while (!this->verifyIfPathIsValid(caravanMovementPath, maxLevelReachable /*+ potentialBoost*/));

    //     this->player->getCaravanAt(caravanMovementPath.at(0)[0], caravanMovementPath.at(0)[1])->move(caravanMovementPath);
    // }

    // /**
    //  * @file Rules.cpp
    //  * @fn bool Rules::verifyIfPathIsValid()
    //  * @brief This function verify if a caravan can travel a certain path depending on it's level.
    //  */
    // bool Rules::verifyIfPathIsValid(std::vector<int[2]> caravanMovementPath, FieldLevel maxLevelReachable)
    // {
    //     long unsigned int i;
    //     for (i = 0; i < caravanMovementPath.size(); i++)
    //     {
    //         if (this->clientGameEngine->mapShared(caravanMovementPath.at(i)[0], caravanMovementPath.at(i)[1])->getFieldLevel() > maxLevelReachable)
    //         {
    //             std::cout << "Invalid movement, you can't reach that level" << std::endl;
    //             return false;
    //         }

    //         // boucle avec l'eau ou les barbares
    //     }
    //     return true;
    // }

    // void Rules::exchangeRessourceWithOtherPlayer()
    // {
    //     std::array<PlayerResource, 2> resourceToExchange;
    //     do
    //     {
    //         resourceToExchange = this->clientGameEngine->exchangeResource();
    //         /**
    //         Create a function in the client game engine that ask the player to exchange a resource with another player.
    //         This function must be named "exchangeResource"
    //         This function must return an array of PlayerResource (first element is the resource to give, second element is the resource to get)
    //         */
    //     } while (resourceToExchange[0].player->haveResource(*(resourceToExchange[0].resource)) && resourceToExchange[1].player->haveResource(*(resourceToExchange[1].resource)));

    //     resourceToExchange[0].player->removeResource(*(resourceToExchange[0].resource));
    //     resourceToExchange[0].player->addResource(*(resourceToExchange[1].resource));
    //     resourceToExchange[1].player->removeResource(*(resourceToExchange[1].resource));
    //     resourceToExchange[1].player->addResource(*(resourceToExchange[0].resource));
    // }

    // void Rules::playAnotherCard()
    // {
    //     CardsEnum cardPlayed;
    //     do
    //     {
    //         std::vector<CardsEnum> availableCards = {CardsEnum::science, CardsEnum::military, CardsEnum::culture, CardsEnum::industry};
    //         cardPlayed = this->clientGameEngine->chooseCard(availableCards);
    //         /**
    //          * Create a function in the client game engine that ask the player to choose a card to play.
    //          * This function must be named "chooseCard"
    //          * This function must return a pointer to a card
    //          */
    //     } while (cardPlayed == CardsEnum::economy);
    //     this->gameEngine->playCard(cardPlayed);
    //     /**
    //      * Create a function in the game engine that play a card.
    //      * This function must be named "playCard"
    //      * This function must take a pointer to a card as parameter
    //      */
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

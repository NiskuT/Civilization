#include <client.hpp>
#include <shared.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <array>

#define RULESLENGTH 16
#define CARAVAN_STEPS_AT_LEVEL_1 3
#define CARAVAN_STEPS_AT_LEVEL_2 4
#define CARAVAN_STEPS_AT_LEVEL_3 6
#define CARAVAN_STEPS_AT_LEVEL_4 6

namespace shared
{

    typedef std::shared_ptr<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>> elementPtr;

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
    void Rules::runTheRule(std::shared_ptr<ArgsOfRule> args)
    {
        switch (this->ruleId)
        {
        case CardsEnum::economy:
            playEconomyCard(args);
            break;
        case CardsEnum::science:
            playScienceCard(args);
            break;
        case CardsEnum::military:
            playMilitaryCard(args);
            break;
        case CardsEnum::culture:
            playCultureCard(args);
            break;
        case CardsEnum::industry:
            playIndustryCard(args);
            break;
        default:
            std::cout << "This rule doesn't have utility" << std::endl;
        }
    }

    /// ECONOMY CARD ///
    /**
     * @file Rules.cpp
     * @fn void Rules::playEconomyCard()
     * @brief This function aim to run the rule of the economy card
     */
    void Rules::playEconomyCard(std::shared_ptr<ArgsOfRule> args)
    {
        // retrieve the arguments
        std::shared_ptr<std::vector<int[2]>> caravanMovementPath = args->economyCardRuleArgs.caravanMovementPath;
        std::shared_ptr<Map> map = args->economyCardRuleArgs.mapOfTheGame;
        unsigned numberOfBoxUsed = args->economyCardRuleArgs.numberOfBoxUsed;

        // check if the player has enough box to play this card
        if (numberOfBoxUsed > this->player->getNumberOfBox(CardsEnum::economy))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        unsigned cardLevel = this->player->getLevelOfCard(CardsEnum::economy);

        std::vector<elementPtr> barbarianList = checkIfBarbarianIsOnThePath(caravanMovementPath, map);
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
                for (int i = 0; i < (int)barbarianList.size(); i++)
                {
                    (std::get<Barbarian>(*barbarianList.at(i))).kill();
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
    std::vector<elementPtr> Rules::checkIfBarbarianIsOnThePath(std::shared_ptr<std::vector<int[2]>> caravanMovementPath, std::shared_ptr<shared::Map> map)
    {
        std::vector<elementPtr> barbarianList;

        for (int i = 0; i < (int)caravanMovementPath->size(); i++)
        {
            unsigned x = caravanMovementPath->at(i)[0];
            unsigned y = caravanMovementPath->at(i)[1];
            elementPtr element = (map->operator()(x, y)->getElement(ElementEnum::barbarian));
            if (std::holds_alternative<Barbarian>(*element))
            {
                barbarianList.push_back(element);
            }
            else
            {
                std::cout << "This is not a barbarian" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        return barbarianList;
    }

    /**
     * @file Rules.cpp
     * @fn void Rules::checkIfWaterIsOnThePath(std::vector<int[2]> *caravanMovementPath)
     * @brief This function aim to check if water is on the path of the caravan
     */
    bool Rules::checkIfWaterIsOnThePath(std::shared_ptr<std::vector<int[2]>> caravanMovementPath, std::shared_ptr<shared::Map> map)
    {
        for (int i = 0; i < (int)caravanMovementPath->size(); i++)
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

    /// SCIENCE CARD ///
    /**
     * @file Rules.cpp
     * @fn void Rules::playScienceCard()
     * @brief This function aim to run the rule of the science card
     */
    void Rules::playScienceCard(std::shared_ptr<ArgsOfRule> args)
    {
        unsigned numberOfBoxUsed = args->scienceCardRuleArgs.numberOfBoxUsed;
        if (numberOfBoxUsed > this->player->getNumberOfBox(CardsEnum::science))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        CardsEnum cardToGetABox = args->scienceCardRuleArgs.cardToGetABox;
        ResourceEnum resourceToGet = args->scienceCardRuleArgs.resourceToGet;
        std::array<unsigned, 2> positionToNuke = args->scienceCardRuleArgs.positionToNuke;
        std::shared_ptr<Map> gameMap = args->scienceCardRuleArgs.gameMap;

        std::vector<std::array<unsigned, 2>> neighbors; // for the nuke

        unsigned cardLevel = this->player->getLevelOfCard(CardsEnum::science);
        switch (cardLevel)
        {
        case 1:
            // do nothing special
            break;
        case 2:
            this->player->addBox(cardToGetABox);
            break;
        case 3:
            if (this->player->haveResource(resourceToGet))
            {
                std::cout << "You already have this resource" << std::endl;
                exit(EXIT_FAILURE);
            }
            this->player->addResource(resourceToGet);
            break;
        case 4: // nuke destroy the control pawn around the position =
            neighbors = getNeighbors(positionToNuke[0], positionToNuke[1], gameMap);
            neighbors.push_back(positionToNuke);
            nuke(neighbors, gameMap);
            break;
        default:
            std::cout << "invalid card level" << std::endl;
            exit(1);
        }
    }

    void Rules::nuke(std::vector<std::array<unsigned, 2>> neightbors, std::shared_ptr<Map> gameMap)
    {
        elementPtr controlPawn;
        for (int i = 0; i < (int)neightbors.size(); i++)
        {
            unsigned x = neightbors.at(i)[0];
            unsigned y = neightbors.at(i)[1];
            controlPawn = gameMap->operator()(x, y)->getElement(ElementEnum::controlPawn);
            if (std::holds_alternative<ControlPawn>(*controlPawn))
            {
                std::get<ControlPawn>(*controlPawn).kill();
            }
            else
            {
                std::cout << "This is not a barbarian" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    std::vector<std::array<unsigned, 2>> Rules::getNeighbors(unsigned posX, unsigned posY, std::shared_ptr<Map> gameMap)
    {
        std::vector<std::array<unsigned, 2>> neighbors;
        unsigned mapSizeX = gameMap->getMapWidth();
        unsigned mapSizeY = gameMap->getMapHeight();
        if (posY > 0)
        {
            neighbors.push_back({posX, posY - 1});
        }
        if (posY > 0 && posX < mapSizeX - 1)
        {
            neighbors.push_back({posX + 1, posY - 1});
        }
        if (posX < mapSizeX - 1)
        {
            neighbors.push_back({posX + 1, posY});
        }
        if (posY < mapSizeY - 1)
        {
            neighbors.push_back({posX, posY + 1});
        }
        if (posY < mapSizeY - 1 && posX > 0)
        {
            neighbors.push_back({posX - 1, posY + 1});
        }
        if (posX > 0)
        {
            neighbors.push_back({posX - 1, posY});
        }
        return neighbors;
    }

    /// CULTURE CARD ///
    void Rules::playCultureCard(std::shared_ptr<ArgsOfRule> args)
    {
        unsigned numberOfBoxUsed = args->scienceCardRuleArgs.numberOfBoxUsed;
        if (numberOfBoxUsed > this->player->getNumberOfBox(CardsEnum::culture))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::vector<std::array<unsigned, 2>> pawnsPositions = args->cultureCardRuleArgs.pawnsPositions;
        std::shared_ptr<Map> gameMap = args->scienceCardRuleArgs.gameMap;

        unsigned cardLevel = this->player->getLevelOfCard(CardsEnum::culture);
        switch (cardLevel)
        {
        case 1:
            if (pawnsPositions.size() == 2 + numberOfBoxUsed)
            {
                placeControlPawns(pawnsPositions, gameMap);
            }
            else
            {
                std::cout << "invalid number of pawn positions" << std::endl;
                exit(1);
            }
            break;
        case 2:
            // TODO : move one pawn on an empty field
            break;
        case 3:
            // TODO : Place one pawn on next to an ally hexagon
            break;
        case 4:
            // TODO: replace enemy pawn (reinforced or not) by one of your pawn (non reinforced)
            break;
        default:
            std::cout << "invalid card level" << std::endl;
            exit(1);
        }
    }

    void Rules::placeControlPawns(std::vector<std::array<unsigned, 2>> positions, std::shared_ptr<Map> gameMap)
    {
        for (auto position : positions)
        {
            if (std::holds_alternative<ControlPawn>(*gameMap->operator()(position[0], position[1])->getElement(ElementEnum::controlPawn)))
            {
                // TODO : create a control pawn
            }
            else
            {
                std::cout << "There is already a control pawn on this position" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    /// MILITARY CARD ///
    void Rules::playMilitaryCard(std::shared_ptr<ArgsOfRule> args)
    {
        if (args->militaryCardRuleArgs.attack == true)
        {
            attack(args);
        }
        else
        {
            reinforce(args);
        }
    }

    void Rules::attack(std::shared_ptr<ArgsOfRule> args) // TODO : remake this function
    {
        // std::shared_ptr<Map> gameMap = args->scienceCardRuleArgs.gameMap;
        // std::vector<std::array<unsigned, 2>> pawnsPositions = args->militaryCardRuleArgs.pawnsPositions;
        // std::shared_ptr<ControlPawn> controlPawn;
        // for (auto position : pawnsPositions)
        // {
        //     controlPawn = gameMap->operator()(position[0], position[1])->getElement(ElementEnum::controlPawn)->controlPawn;
        //     if (controlPawn != nullptr)
        //     {
        //         controlPawn->kill();
        //     }
        // }
    }

    void Rules::reinforce(std::shared_ptr<ArgsOfRule> args)
    {
        unsigned numberOfBoxUsed = args->scienceCardRuleArgs.numberOfBoxUsed;
        if (numberOfBoxUsed > this->player->getNumberOfBox(CardsEnum::military))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::shared_ptr<Map> gameMap = args->scienceCardRuleArgs.gameMap;
        std::vector<std::array<unsigned, 2>> pawnsPositions = args->militaryCardRuleArgs.pawnsPositions;
        unsigned cardLevel = this->player->getLevelOfCard(CardsEnum::culture);
        if (pawnsPositions.size() != cardLevel + numberOfBoxUsed)
        {
            std::cout << "invalid number of pawn positions" << std::endl;
            exit(1);
        }
        std::shared_ptr<ControlPawn> controlPawn;
        for (auto position : pawnsPositions)
        {
            controlPawn = gameMap->operator()(position[0], position[1])->getElement(ElementEnum::controlPawn)->controlPawn;
            if (controlPawn != nullptr)
            {
                controlPawn->reinforce();
            }
        }
    }

    /// INDUSTRY CARD ///
    void Rules::playIndustryCard(std::shared_ptr<ArgsOfRule> args)
    {
        if (args->industryCardRuleArgs.buildWonder == true)
        {
            buildWonder(args);
        }
        else
        {
            buildCity(args);
        }
    }

    void Rules::buildWonder(std::shared_ptr<ArgsOfRule> args) // TODO : remake this function
    {
        std::shared_ptr<Map> gameMap = args->scienceCardRuleArgs.gameMap;
        std::array<unsigned, 2> position = args->industryCardRuleArgs.position;
        // std::shared_ptr<Wonder> wonder = gameMap->operator()(position[0], position[1])->getElement(ElementEnum::wonder)->wonder;
        // if (wonder != nullptr)
        // {
        //     wonder->build();
        // }
    }

    void Rules::buildCity(std::shared_ptr<ArgsOfRule> args) // TODO : remake this function
    {
        unsigned numberOfBoxUsed = args->scienceCardRuleArgs.numberOfBoxUsed;
        if (numberOfBoxUsed > this->player->getNumberOfBox(CardsEnum::industry))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::shared_ptr<Map> gameMap = args->scienceCardRuleArgs.gameMap;
        std::array<unsigned, 2> position = args->industryCardRuleArgs.position;
        std::shared_ptr<City> city = gameMap->operator()(position[0], position[1])->getElement(ElementEnum::city)->city;
        if (city != nullptr)
        {
            city->build();
        }
    }

}

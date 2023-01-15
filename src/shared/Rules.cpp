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

    typedef std::vector<std::shared_ptr<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>> elementList;

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
    void Rules::runTheRule(RuleArgsStruct &args)
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
    void Rules::playEconomyCard(RuleArgsStruct &args)
    {
        std::shared_ptr<std::vector<std::array<int, 2>>> caravanMovementPath = args.caravanMovementPath;
        std::shared_ptr<Map> map = args.gameMap;
        unsigned numberOfBoxUsed = args.numberOfBoxUsed;

        if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::economy))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::economy);

        elementList barbarianList = checkIfBarbarianIsOnThePath(caravanMovementPath, map);
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
        for (auto &element : *caravanMovementPath)
        {
            if (map->operator()(element[0], element[1])->getFieldLevel() > (FieldLevel)maxLevelReachable)
            {
                std::cout << "You can't move your caravan on this field" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        std::vector<std::shared_ptr<Caravan>> caravans = args.currentPlayer->getCaravans();

        switch (cardLevel)
        {
        case 1:
            if (caravanMovementPath->size() > CARAVAN_STEPS_AT_LEVEL_1 + numberOfBoxUsed)
            {
                std::cout << "You can't move your caravan of " << CARAVAN_STEPS_AT_LEVEL_1 + numberOfBoxUsed << " steps" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (caravans.size() == 0)
            {
                Caravan newCaravan = Caravan(NULL, NULL);
                args.currentPlayer->addCaravan(std::make_shared<Caravan>(newCaravan));
            }

            moveCaravan(caravans, caravanMovementPath->at(0), caravanMovementPath->at(caravanMovementPath->size() - 1), map);

            break;
        case 2:
            if (caravanMovementPath->size() > CARAVAN_STEPS_AT_LEVEL_2 + numberOfBoxUsed)
            {
                std::cout << "You can't move your caravan of " << CARAVAN_STEPS_AT_LEVEL_1 + numberOfBoxUsed << " steps" << std::endl;
                exit(EXIT_FAILURE);
            }
            while (caravans.size() < 2)
            {
                Caravan newCaravan = Caravan(NULL, NULL);
                args.currentPlayer->addCaravan(std::make_shared<Caravan>(newCaravan));
                caravans.push_back(std::make_shared<Caravan>(newCaravan));
            }
            if (barbarianList.size() > 0)
            {
                for (int i = 0; i < (int)barbarianList.size(); i++)
                {
                    (std::get<Barbarian>(*barbarianList.at(i))).kill();
                    args.currentPlayer->addBox(CardsEnum::economy); // TODO: allow to add box on other card
                }
            }
            moveCaravan(caravans, caravanMovementPath->at(0), caravanMovementPath->at(caravanMovementPath->size() - 1), map);
            moveCaravan(caravans, caravanMovementPath->at(0), caravanMovementPath->at(caravanMovementPath->size() - 1), map);
            break;
        case 3:
            if (caravanMovementPath->size() > CARAVAN_STEPS_AT_LEVEL_3 + numberOfBoxUsed)
            {
                std::cout << "You can't move your caravan of " << CARAVAN_STEPS_AT_LEVEL_1 + numberOfBoxUsed << " steps" << std::endl;
                exit(EXIT_FAILURE);
            }
            while (caravans.size() < 2)
            {
                Caravan newCaravan = Caravan(NULL, NULL);
                args.currentPlayer->addCaravan(std::make_shared<Caravan>(newCaravan));
                caravans.push_back(std::make_shared<Caravan>(newCaravan));
            }
            moveCaravan(caravans, caravanMovementPath->at(0), caravanMovementPath->at(caravanMovementPath->size() - 1), map);
            moveCaravan(caravans, caravanMovementPath->at(0), caravanMovementPath->at(caravanMovementPath->size() - 1), map);
            exchangeRessourceWithOtherPlayer(); //  TODO: allow to exchange ressource with other player
            break;
        case 4:
            if (caravanMovementPath->size() > CARAVAN_STEPS_AT_LEVEL_4 + numberOfBoxUsed)
            {
                std::cout << "You can't move your caravan of " << CARAVAN_STEPS_AT_LEVEL_1 + numberOfBoxUsed << " steps" << std::endl;
                exit(EXIT_FAILURE);
            }
            while (caravans.size() < 3)
            {
                Caravan newCaravan = Caravan(NULL, NULL);
                args.currentPlayer->addCaravan(std::make_shared<Caravan>(newCaravan));
                caravans.push_back(std::make_shared<Caravan>(newCaravan));
            }
            moveCaravan(caravans, caravanMovementPath->at(0), caravanMovementPath->at(caravanMovementPath->size() - 1), map);
            moveCaravan(caravans, caravanMovementPath->at(0), caravanMovementPath->at(caravanMovementPath->size() - 1), map);
            moveCaravan(caravans, caravanMovementPath->at(0), caravanMovementPath->at(caravanMovementPath->size() - 1), map);
            playAnotherCard(); // TODO: allow to play another card
            break;
        default:
            std::cout << "invalid card level" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    bool Rules::moveCaravan(std::vector<std::shared_ptr<Caravan>> caravans, std::array<int, 2> pos1, std::array<int, 2> pos2, std::shared_ptr<Map> map)
    {
        for (auto caravan : caravans)
        {
            if (caravan->getPosX() == pos1[0] && caravan->getPosY() == pos1[1])
            {
                caravan->setPos(pos2[0], pos2[1]);
                map->operator()(pos1[0], pos1[1])->removeElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
                map->operator()(pos2[0], pos2[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
                return true;
            }
        }

        // there is no caravan at the starting point of the path.
        for (auto caravan : caravans)
        {
            if (caravan->getPosX() == NULL && caravan->getPosY() == NULL)
            {
                caravan->setPos(pos2[0], pos2[1]);
                map->operator()(pos2[0], pos2[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
                return true;
            }
        }
        return false;
    }

    /**
     * @file Rules.cpp
     * @fn void Rules::checkIfBarbarianIsOnThePath(std::vector<int[2]> *caravanMovementPath)
     * @brief This function aim to check if a barbarian is on the path of the caravan
     */
    elementList Rules::checkIfBarbarianIsOnThePath(std::shared_ptr<std::vector<std::array<int, 2>>> caravanMovementPath, std::shared_ptr<shared::Map> map)
    {
        elementList barbarianList;

        for (int i = 0; i < (int)caravanMovementPath->size(); i++)
        {
            unsigned x = caravanMovementPath->at(i)[0];
            unsigned y = caravanMovementPath->at(i)[1];
            elementList elements = (map->operator()(x, y)->getElements());
            for (auto element : elements)
            {
                if (std::holds_alternative<Barbarian>(*element))
                {
                    barbarianList.push_back(element);
                }
            }
        }

        return barbarianList;
    }

    /**
     * @file Rules.cpp
     * @fn void Rules::checkIfWaterIsOnThePath(std::vector<int[2]> *caravanMovementPath)
     * @brief This function aim to check if water is on the path of the caravan
     */
    bool Rules::checkIfWaterIsOnThePath(std::shared_ptr<std::vector<std::array<int, 2>>> caravanMovementPath, std::shared_ptr<shared::Map> map)
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
    void Rules::playScienceCard(RuleArgsStruct &args)
    {
        unsigned numberOfBoxUsed = args.numberOfBoxUsed;
        if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::science))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        CardsEnum cardToGetABox = args.cardToGetABox;
        ResourceEnum resourceToGet = args.resourceToGet;
        std::array<unsigned, 2> positionToNuke = args.positionToNuke;
        std::shared_ptr<Map> gameMap = args.gameMap;

        std::vector<std::array<unsigned, 2>> neighbors; // for the nuke

        unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::science);
        switch (cardLevel)
        {
        case 1:
            // do nothing special
            break;
        case 2:
            args.currentPlayer->addBox(cardToGetABox);
            break;
        case 3:
            if (args.currentPlayer->haveResource(resourceToGet))
            {
                std::cout << "You already have this resource" << std::endl;
                exit(EXIT_FAILURE);
            }
            args.currentPlayer->addResource(resourceToGet);
            break;
        case 4:
            neighbors = getNeighbors(positionToNuke[0], positionToNuke[1], gameMap);
            neighbors.push_back(positionToNuke);
            nuke(neighbors, gameMap, args.currentPlayer);
            break;
        default:
            std::cout << "invalid card level" << std::endl;
            exit(1);
        }
    }

    void Rules::nuke(std::vector<std::array<unsigned, 2>> neightbors, std::shared_ptr<Map> gameMap, std::shared_ptr<Player> currentPlayer)
    {
        elementList elements;
        for (int i = 0; i < (int)neightbors.size(); i++)
        {
            unsigned x = neightbors.at(i)[0];
            unsigned y = neightbors.at(i)[1];
            elements = gameMap->operator()(x, y)->getElements();
            for (auto element : elements)
            {
                if (std::holds_alternative<ControlPawn>(*element))
                {
                    if (std::get<ControlPawn>(*element).isReinforced())
                    {
                        std::get<ControlPawn>(*element).setReinforced();
                    }
                    else
                    {
                        gameMap->operator()(x, y)->removeElement(element);
                        currentPlayer->removeControlPawn(std::make_shared<ControlPawn>(std::get<ControlPawn>(*element)));
                    }
                }
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
    void Rules::playCultureCard(RuleArgsStruct &args)
    {
        unsigned numberOfBoxUsed = args.numberOfBoxUsed;
        if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::culture))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::vector<std::array<unsigned, 2>> pawnsPositions = args.pawnsPositions;
        std::shared_ptr<Map> gameMap = args.gameMap;

        unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::culture);
        switch (cardLevel)
        {
        case 1:
            if (pawnsPositions.size() == 2 + numberOfBoxUsed)
            {
                placeControlPawns(pawnsPositions, gameMap, args.currentPlayer);
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

    void Rules::placeControlPawns(std::vector<std::array<unsigned, 2>> positions, std::shared_ptr<Map> gameMap, std::shared_ptr<Player> currentPlayer) // TODO : split this function
    {
        for (auto position : positions)
        {
            for (auto element : gameMap->operator()(position[0], position[1])->getElements())
            {
                if (std::holds_alternative<ControlPawn>(*element))
                {
                    std::cout << "There is already a control pawn on this position" << std::endl;
                    exit(EXIT_FAILURE);
                }
                else
                {
                    std::vector<std::array<unsigned, 2>> neighbors = getNeighbors(position[0], position[1], gameMap);
                    for (auto neighbor : neighbors)
                    {
                        for (auto element2 : gameMap->operator()(neighbor[0], neighbor[1])->getElements())
                        {
                            if (std::holds_alternative<City>(*element2))
                            {
                                ControlPawn controlPawn;

                                gameMap->operator()(position[0], position[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(controlPawn));
                                currentPlayer->addControlPawn(std::make_shared<ControlPawn>(controlPawn));
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    /// MILITARY CARD ///
    void Rules::playMilitaryCard(RuleArgsStruct &args)
    {
        if (args.militaryCardAttack == true)
        {
            attack(args);
        }
        else
        {
            reinforce(args);
        }
    }

    void Rules::attack(RuleArgsStruct &args) // TODO : remake this function
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

    void Rules::reinforce(RuleArgsStruct &args)
    {
        unsigned numberOfBoxUsed = args.numberOfBoxUsed;
        if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::military))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::shared_ptr<Map> gameMap = args.gameMap;
        std::vector<std::array<unsigned, 2>> pawnsPositions = args.pawnsPositions;
        unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::culture);
        if (pawnsPositions.size() != cardLevel + numberOfBoxUsed)
        {
            std::cout << "invalid number of pawn positions" << std::endl;
            exit(1);
        }
        std::shared_ptr<ControlPawn> controlPawn;
        for (auto position : pawnsPositions)
        {
            for (auto element : gameMap->operator()(position[0], position[1])->getElements())
            {
                if (std::holds_alternative<ControlPawn>(*element))
                {
                    std::get<ControlPawn>(*element).setReinforced();
                    return;
                }
            }
            std::cout << "There is no control pawn on this position" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    /// INDUSTRY CARD ///
    void Rules::playIndustryCard(RuleArgsStruct &args)
    {
        if (args.industryCardBuildWonder == true)
        {
            buildWonder(args);
        }
        else
        {
            buildCity(args);
        }
    }

    void Rules::buildWonder(RuleArgsStruct &args) // TODO : remake this function
    {
        // std::shared_ptr<Map> gameMap = args.gameMap;
        // std::array<unsigned, 2> position = args.positionOfWonder;
        // std::shared_ptr<Wonder> wonder = gameMap->operator()(position[0], position[1])->getElement(ElementEnum::wonder)->wonder;
        // if (wonder != nullptr)
        // {
        //     wonder->build();
        // }
    }

    void Rules::buildCity(RuleArgsStruct &args) // TODO : remake this function
    {
        unsigned numberOfBoxUsed = args.numberOfBoxUsed;
        if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::industry))
        {
            std::cout << "You don't have enough box to play this card" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::shared_ptr<Map> gameMap = args.gameMap;
        std::array<unsigned, 2> position = args.positionOfCity;

        for (auto element : gameMap->operator()(position[0], position[1])->getElements())
        {
            if (std::holds_alternative<City>(*element))
            {
                std::cout << "There is already a city on this position" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        // TODO : create a city
    }
}

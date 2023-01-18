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

using namespace shared;

typedef std::vector<std::shared_ptr<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>> elementList;

/**
 * @file Rules.cpp
 * @fn Rules::Rules(unsigned ruleId)
 * @brief constructor of Rules, define the ruleId of created rule.
 */
Rules::Rules()
{
}

/**
 * @file Rules.cpp
 * @fn void Rules::runTheRule(unsigned numberOfBoxUsed)
 * @brief This function aim to run the rule of the card
 * @param args : struct containing all the arguments needed to run the rule
 */
void Rules::runTheRule(RuleArgsStruct &args)
{
    switch (args.ruleId)
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
    // case CardsEnum::culture:
    //     playCultureCard(args);
    //     break;
    // case CardsEnum::industry:
    //     playIndustryCard(args);
    //     break;
    default:
        std::cout << "This rule doesn't have utility" << std::endl;
    }
}

/// ECONOMY CARD ///
/**
 * @file Rules.cpp
 * @fn void Rules::playEconomyCard()
 * @brief This function aim to run the rule of the economy card
 * @param args : struct containing all the arguments needed to run the rule
 */
void Rules::playEconomyCard(RuleArgsStruct &args)
{
    std::vector<std::array<unsigned, 2>> caravanMovementPath = args.caravanMovementPath;
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
    for (auto &element : caravanMovementPath)
    {
        if ((*map)(element[0], element[1])->getFieldLevel() > (FieldLevel)maxLevelReachable)
        {
            std::cout << "You can't move your caravan on this field" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::vector<std::shared_ptr<Caravan>> caravans = args.currentPlayer->getCaravans();

    switch (cardLevel)
    {
    case 1:
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, 1);
        verifyIfPathSizeIsCorrect(caravanMovementPath.size(), CARAVAN_STEPS_AT_LEVEL_1 + numberOfBoxUsed);
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, 1);

        moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        break;
    case 2:
        verifyIfPathSizeIsCorrect(caravanMovementPath.size(), CARAVAN_STEPS_AT_LEVEL_2 + numberOfBoxUsed);
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, 2);
        if (barbarianList.size() > 0)
        {
            for (int i = 0; i < (int)barbarianList.size(); i++)
            {
                (std::get<Barbarian>(*barbarianList.at(i))).kill();
                args.currentPlayer->addBox(CardsEnum::economy, 1);
            }
        }
        moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // TODO : make caravanMovementPath a vector of vector to allow to move several caravans
        // moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        break;
    case 3:
        verifyIfPathSizeIsCorrect(caravanMovementPath.size(), CARAVAN_STEPS_AT_LEVEL_3 + numberOfBoxUsed);
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, 2);
        moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // exchangeRessourceWithOtherPlayer(); //  TODO: allow to exchange ressource with other player
        break;
    case 4:
        verifyIfPathSizeIsCorrect(caravanMovementPath.size(), CARAVAN_STEPS_AT_LEVEL_4 + numberOfBoxUsed);
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, 3);
        moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // playAnotherCard(); // TODO: allow to play another card
        break;
    default:
        std::cout << "invalid card level" << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * @file Rules.cpp
 * @fn void Rules::verifyIfPathSizeIsCorrect(int pathSize, int maxPathSize)
 * @brief This function aim to verify if the path size is correct
 * @param pathSize the size of the path
 * @param maxPathSize the max size of the path
 */
void Rules::verifyIfPathSizeIsCorrect(int pathSize, int maxPathSize)
{
    if (pathSize > maxPathSize)
    {
        std::cout << "You can't move your caravan of " << maxPathSize << " steps" << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * @file Rules.cpp
 * @fn void Rules::addCaravanAfterCardAmelioration(std::vector<std::shared_ptr<Caravan>> caravans, std::shared_ptr<Player> currentPlayer, int numberOfCaravans)
 * @brief This function aim to add caravan after the card amelioration
 * @param caravans the caravans of the player
 * @param currentPlayer the current player
 * @param numberOfCaravans the number of caravans related to the card level
 */
void Rules::addCaravanAfterCardAmelioration(std::vector<std::shared_ptr<Caravan>> &caravans, std::shared_ptr<Player> currentPlayer, int numberOfCaravans)
{
    while (caravans.size() < numberOfCaravans)
    {
        std::array<unsigned, 2> pos = {0, 0};
        std::shared_ptr<Caravan> caravan = std::make_shared<Caravan>(pos);
        currentPlayer->addCaravan(caravan);
        caravans.push_back(caravan);
    }
}

/**
 * @file Rules.cpp
 * @fn void Rules::moveCaravan(std::vector<std::shared_ptr<Caravan>> caravans, std::array<int, 2> pos1, std::array<int, 2> pos2, std::shared_ptr<Map> map)
 * @brief This function aim to move the caravan
 * @param caravans the caravans of the player
 * @param pos1 the first position of the caravan
 * @param pos2 the second position of the caravan
 * @param map the map
 *
 */
void Rules::moveCaravan(std::vector<std::shared_ptr<Caravan>> caravans, std::array<unsigned, 2> pos1, std::array<unsigned, 2> pos2, std::shared_ptr<Map> map, std::shared_ptr<Player> currentPlayer)
{
    for (auto caravan : caravans)
    {
        if (caravan->getPosition() == pos1 && caravan->isUsed() == true)
        {
            caravan->setPos(pos2);
            (*map)(pos1[0], pos1[1])->removeElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
            (*map)(pos2[0], pos2[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
            return;
        }
    }

    // there is no caravan at the starting point of the path.
    // TODO : check if there is a city or a controlpawn next to this position
    std::vector<std::array<unsigned, 2>> neighbors;
    neighbors = getNeighbors((unsigned)pos1[0], (unsigned)pos1[1], map);
    bool isThereACityOrAControlPawn = false;
    for (auto city : currentPlayer->getCityList())
    {
        if (std::find(neighbors.begin(), neighbors.end(), city->getPosition()) != neighbors.end())
        {
            isThereACityOrAControlPawn = true;
            break;
        }
    }
    for (auto controlPawn : currentPlayer->getControlPawns())
    {
        if (std::find(neighbors.begin(), neighbors.end(), controlPawn->getPosition()) != neighbors.end() || isThereACityOrAControlPawn)
        {
            isThereACityOrAControlPawn = true;
            break;
        }
    }
    if (!isThereACityOrAControlPawn)
    {
        std::cout << "You can't move your caravan if there is no city or control pawn next to the starting point of the path" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (auto caravan : caravans)
    {
        if (!(caravan->isUsed()))
        {
            caravan->setPos(pos2);
            caravan->setUsed(true);
            (*map)(pos2[0], pos2[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
            return;
        }
    }
}

/**
 * @file Rules.cpp
 * @fn void Rules::checkIfBarbarianIsOnThePath(std::vector<int[2]> *caravanMovementPath)
 * @brief This function aim to check if a barbarian is on the path of the caravan
 * @param caravanMovementPath the path of the caravan
 * @param map the map of the game
 * @return a list of barbarian
 */
elementList Rules::checkIfBarbarianIsOnThePath(std::vector<std::array<unsigned, 2>> caravanMovementPath, std::shared_ptr<shared::Map> map)
{
    elementList barbarianList;

    for (int i = 0; i < (int)caravanMovementPath.size(); i++)
    {
        elementList elements = (*map)(caravanMovementPath.at(i)[0], caravanMovementPath.at(i)[1])->getElements();
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
 * @param caravanMovementPath the path of the caravan
 * @param map the map of the game
 * @return true if there is water on the path, false otherwise
 */
bool Rules::checkIfWaterIsOnThePath(std::vector<std::array<unsigned, 2>> caravanMovementPath, std::shared_ptr<shared::Map> map)
{
    for (auto position : caravanMovementPath)
    {
        if ((*map)(position[0], position[1])->getFieldLevel() == FieldLevel::Water)
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
 * @param args : struct containing all the arguments needed to run the rule
 */
void Rules::playScienceCard(RuleArgsStruct &args)
{
    std::shared_ptr<Player> currentPlayer = args.currentPlayer;
    unsigned numberOfBoxUsed = args.numberOfBoxUsed;
    if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::science))
    {
        std::cout << "You don't have enough box to play this card" << std::endl;
        exit(EXIT_FAILURE);
    }
    args.currentPlayer->deleteBox(CardsEnum::science, numberOfBoxUsed);

    CardsEnum cardToGetABox;
    ResourceEnum resourceToGet;
    std::array<unsigned, 2> positionToNuke;
    std::shared_ptr<Map> gameMap;
    std::vector<CardsEnum> cardsToImprove = args.cardsToImprove;

    std::vector<std::array<unsigned, 2>> neighbors; // for the nuke

    unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::science);
    std::array<int, 3> cardsLevelToImprove = currentPlayer->incrementTechWheel(cardLevel + numberOfBoxUsed);
    for (auto cardtoImprove : cardsToImprove)
    {
        if (cardsLevelToImprove[currentPlayer->getLevelOfCard(cardtoImprove) - 1] > 0)
        {
            cardsLevelToImprove[currentPlayer->getLevelOfCard(cardtoImprove) - 1]--;
            args.currentPlayer->upgradeCard(cardtoImprove);
        }
    }
    switch (cardLevel)
    {
    case 1:
        // do nothing special
        break;
    case 2:
        cardToGetABox = args.cardToGetABox;
        args.currentPlayer->addBox(cardToGetABox, 1);
        break;
    case 3:
        resourceToGet = args.resourceToGet;
        if (args.currentPlayer->haveResource(resourceToGet))
        {
            std::cout << "You already have this resource" << std::endl;
            exit(EXIT_FAILURE);
        }
        args.currentPlayer->addResource(resourceToGet);
        break;
    case 4:
        positionToNuke = args.positionToNuke;
        gameMap = args.gameMap;
        neighbors = getNeighbors(positionToNuke[0], positionToNuke[1], gameMap);
        neighbors.push_back(positionToNuke);
        nuke(neighbors, gameMap, args.currentPlayer);
        break;
    default:
        std::cout << "invalid card level" << std::endl;
        exit(1);
    }
}

/**
 * @file Rules.cpp
 * @fn void Rules::nuke(std::vector<std::array<unsigned, 2>> neightbors, std::shared_ptr<Map> gameMap, std::shared_ptr<Player> currentPlayer)
 * @brief This function aim to nuke the position given in parameter
 * @param neightbors the positions to nuke
 * @param gameMap the map of the game
 * @param currentPlayer the current player
 */
void Rules::nuke(std::vector<std::array<unsigned, 2>> neightbors, std::shared_ptr<Map> gameMap, std::shared_ptr<Player> currentPlayer)
{
    elementList elements;
    for (auto neightbor : neightbors)
    {
        elements = (*gameMap)(neightbor[0], neightbor[1])->getElements();
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
                    (*gameMap)(neightbor[0], neightbor[1])->removeElement(element);
                    currentPlayer->removeControlPawn(std::make_shared<ControlPawn>(std::get<ControlPawn>(*element)));
                }
            }
        }
    }
}

/**
 * @file Rules.cpp
 * @fn std::vector<std::array<unsigned, 2>> Rules::getNeighbors(unsigned posX, unsigned posY, std::shared_ptr<Map> gameMap)
 * @brief This function aim to get the neighbors of a position
 * @param posX the x position
 * @param posY the y position
 * @param gameMap the map of the game
 * @return the neighbors of the position
 */
std::vector<std::array<unsigned, 2>> Rules::getNeighbors(unsigned posX, unsigned posY, std::shared_ptr<Map> gameMap)
{
    std::vector<std::array<unsigned, 2>> neighbors = {};
    unsigned mapSizeX = gameMap->getMapWidth();
    unsigned mapSizeY = gameMap->getMapHeight();
    if (posY % 2)
    {
        if (posY > 0)
        {
            neighbors.push_back({posX, posY - 1});
        }
        if (posY > 0 && posX > 0)
        {
            neighbors.push_back({posX - 1, posY - 1});
        }
        if (posX > 0)
        {
            neighbors.push_back({posX - 1, posY});
        }
        if (posY < mapSizeY - 1)
        {
            neighbors.push_back({posX, posY + 1});
        }
        if (posY < mapSizeY - 1 && posX > 0)
        {
            neighbors.push_back({posX - 1, posY + 1});
        }
        if (posX < mapSizeX - 1)
        {
            neighbors.push_back({posX + 1, posY});
        }
    }
    else
    {
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
    }
    return neighbors;
}

// /// CULTURE CARD ///
// /**
//  * @file Rules.cpp
//  * @fn void Rules::playCultureCard(RuleArgsStruct &args)
//  * @brief This function aim to play the culture card
//  * @param args : struct containing all the arguments needed to run the rule
//  */
// void Rules::playCultureCard(RuleArgsStruct &args)
// {
//     unsigned numberOfBoxUsed = args.numberOfBoxUsed;
//     if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::culture))
//     {
//         std::cout << "You don't have enough box to play this card" << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     std::vector<std::array<unsigned, 2>> pawnsPositions = args.pawnsPositions;
//     std::shared_ptr<Map> gameMap = args.gameMap;

//     unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::culture);
//     switch (cardLevel)
//     {
//     case 1:
//         if (pawnsPositions.size() == 2 + numberOfBoxUsed)
//         {
//             placeControlPawns(pawnsPositions, gameMap, args.currentPlayer);
//         }
//         else
//         {
//             std::cout << "invalid number of pawn positions" << std::endl;
//             exit(1);
//         }
//         break;
//     case 2:
//         // TODO : move one pawn on an empty field
//         break;
//     case 3:
//         // TODO : Place one pawn on next to an ally hexagon
//         break;
//     case 4:
//         // TODO: replace enemy pawn (reinforced or not) by one of your pawn (non reinforced)
//         break;
//     default:
//         std::cout << "invalid card level" << std::endl;
//         exit(1);
//     }
// }

// /**
//  * @file Rules.cpp
//  * @fn void Rules::placeControlPawns(std::vector<std::array<unsigned, 2>> positions, std::shared_ptr<Map> gameMap, std::shared_ptr<Player> currentPlayer)
//  * @brief This function aim to place the control pawns on the map
//  * @param positions the positions where the control pawns will be placed
//  * @param gameMap the map of the game
//  * @param currentPlayer the current player that owns the control pawns
//  */
// void Rules::placeControlPawns(std::vector<std::array<unsigned, 2>> positions, std::shared_ptr<Map> gameMap, std::shared_ptr<Player> currentPlayer) // TODO : split this function
// {
//     for (auto position : positions)
//     {
//         for (auto element : (*gameMap)(position[0], position[1])->getElements())
//         {
//             if (std::holds_alternative<ControlPawn>(*element))
//             {
//                 std::cout << "There is already a control pawn on this position" << std::endl;
//                 exit(EXIT_FAILURE);
//             }
//             if (isThereACityAround(position, gameMap))
//             {
//                 ControlPawn controlPawn;

//                 (*gameMap)(position[0], position[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(controlPawn));
//                 currentPlayer->addControlPawn(std::make_shared<ControlPawn>(controlPawn));
//             }
//         }
//     }
// }

// /**
//  * @file Rules.cpp
//  * @fn bool Rules::isThereACityAround(std::array<unsigned, 2> position, std::shared_ptr<Map> gameMap)
//  * @brief This function aim to check if there is a city around a position
//  * @param position the position to check
//  * @param gameMap the map of the game
//  * @return true if there is a city around, false otherwise
//  */
// bool Rules::isThereACityAround(std::array<unsigned, 2> position, std::shared_ptr<Map> gameMap)
// {

//     std::vector<std::array<unsigned, 2>> neighbors = getNeighbors(position[0], position[1], gameMap);
//     for (auto neighbor : neighbors)
//     {
//         for (auto element2 : (*gameMap)(neighbor[0], neighbor[1])->getElements())
//         {
//             if (std::holds_alternative<City>(*element2))
//             {
//                 return true; // TODO : remove this return
//             }
//         }
//     }
//     return false;
// }

/// MILITARY CARD ///
/**
 * @file Rules.cpp
 * @fn void Rules::playMilitaryCard(RuleArgsStruct &args)
 * @brief This function aim to play the military card
 * @param args : struct containing all the arguments needed to run the rule
 */
void Rules::playMilitaryCard(RuleArgsStruct &args)
{
    std::cout << "playMilitaryCard" << std::endl;
    if (args.militaryCardAttack == true)
    {
        attack(args);
    }
    else
    {
        reinforce(args);
    }
}

/**
 * @file Rules.cpp
 * @fn void Rules::attack(RuleArgsStruct &args)
 * @brief This function aim to attack an enemy pawn or a city
 * @param args : struct containing all the arguments needed to run the rule
 */
void Rules::attack(RuleArgsStruct &args)
{
    // TODO : attack
    // I have problems mesuring the distance of the enemy pawn from the current player hexagons
}

/**
 * @file Rules.cpp
 * @fn void Rules::reinforce(RuleArgsStruct &args)
 * @brief This function aim to reinforce a pawn
 * @param args : struct containing all the arguments needed to run the rule
 */
void Rules::reinforce(RuleArgsStruct &args)
{
    std::cout << "0" << std::endl;
    unsigned numberOfBoxUsed = args.numberOfBoxUsed;
    if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::military))
    {
        std::cout << "You don't have enough box to play this card" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "1" << std::endl;
    std::shared_ptr<Map> gameMap = args.gameMap;
    std::vector<std::array<unsigned, 2>> pawnsPositions = args.pawnsPositions;
    unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::military);
    if (pawnsPositions.size() != cardLevel + numberOfBoxUsed)
    {
        std::cout << "invalid number of pawn positions" << std::endl;
        exit(1);
    }
    std::shared_ptr<ControlPawn> controlPawn;

    std::cout << "2" << std::endl;
    for (auto position : pawnsPositions)
    {
        for (auto element : (*gameMap)(position[0], position[1])->getElements())
        {
            if (std::holds_alternative<ControlPawn>(*element))
            {
                std::cout << "test tes ttes ests ets etset" << std::endl;
                std::get<ControlPawn>(*element).setReinforced();
                break;
            }
        }
    }
}

// /// INDUSTRY CARD ///
// /**
//  * @file Rules.cpp
//  * @fn void Rules::playIndustryCard(RuleArgsStruct &args)
//  * @brief This function aim to play the industry card
//  * @param args : struct containing all the arguments needed to run the rule
//  */
// void Rules::playIndustryCard(RuleArgsStruct &args)
// {
//     if (args.industryCardBuildWonder == true)
//     {
//         buildWonder(args);
//     }
//     else
//     {
//         buildCity(args);
//     }
// }

// /**
//  * @file Rules.cpp
//  * @fn void Rules::buildWonder(RuleArgsStruct &args)
//  * @brief This function aim to build a wonder
//  * @param args : struct containing all the arguments needed to run the rule
//  */
// void Rules::buildWonder(RuleArgsStruct &args) // TODO : remake this function
// {
//     // TODO : build a wonder
// }

// /**
//  * @file Rules.cpp
//  * @fn void Rules::buildCity(RuleArgsStruct &args)
//  * @brief This function aim to build a city
//  * @param args : struct containing all the arguments needed to run the rule
//  */
// void Rules::buildCity(RuleArgsStruct &args) // TODO : remake this function
// {
//     unsigned numberOfBoxUsed = args.numberOfBoxUsed;
//     if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::industry))
//     {
//         std::cout << "You don't have enough box to play this card" << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     std::shared_ptr<Map> gameMap = args.gameMap;
//     std::array<unsigned, 2> position = args.positionOfCity;

//     for (auto element : (*gameMap)(position[0], position[1])->getElements())
//     {
//         if (!(std::holds_alternative<Caravan>(*element)))
//         {
//             std::cout << "There is already an Element on this position" << std::endl;
//             exit(EXIT_FAILURE);
//         }
//     }
//     // TODO : create a city
//     // I have problems mesuring the distance
// }

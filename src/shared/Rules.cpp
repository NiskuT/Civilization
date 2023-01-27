#include <shared.hpp>
#include <algorithm>
#include <vector>
#include <array>
#include <iostream>

#define RULESLENGTH 16
#define CARAVAN_STEPS_AT_LEVEL_1 4
#define CARAVAN_STEPS_AT_LEVEL_2 5
#define CARAVAN_STEPS_AT_LEVEL_3 7
#define CARAVAN_STEPS_AT_LEVEL_4 7

#define CARVAN_NUMBER_AT_LEVEL_1 1
#define CARVAN_NUMBER_AT_LEVEL_2 2
#define CARVAN_NUMBER_AT_LEVEL_3 2
#define CARVAN_NUMBER_AT_LEVEL_4 3

#define NUMBER_PAWNS_ECONOMY_LVL_1 2

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
 * @fn void Rules::runTheRule(RuleArgsStruct &args)
 * @brief This function aim to run the rule of the card
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::runTheRule(RuleArgsStruct &args)
{
    switch (args.ruleId)
    {
    case CardsEnum::economy:
        return playEconomyCard(args);
    case CardsEnum::science:
        return playScienceCard(args);
    case CardsEnum::military:
        return playMilitaryCard(args);
    case CardsEnum::culture:
        return playCultureCard(args);
    case CardsEnum::industry:
        return playIndustryCard(args);
    default:
        return false;
    }
}

/// ECONOMY CARD ///
/**
 * @file Rules.cpp
 * @fn void Rules::playEconomyCard(RuleArgsStruct &args)
 * @brief This function aim to run the rule of the economy card
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::playEconomyCard(RuleArgsStruct &args)
{
    std::vector<std::array<unsigned, 2>> caravanMovementPath = args.caravanMovementPath;
    std::shared_ptr<Map> map = args.gameMap;
    unsigned numberOfBoxUsed = args.numberOfBoxUsed;

    if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::economy))
    {
        return false;
    }

    unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::economy);

    elementList barbarianList = checkIfBarbarianIsOnThePath(caravanMovementPath, map);
    if (barbarianList.size() > 0 && cardLevel != 2)
    {
        return false;
    }

    bool isWaterOnThePath = checkIfWaterIsOnThePath(caravanMovementPath, map);
    if (isWaterOnThePath && cardLevel < 3)
    {
        return false;
    }

    unsigned maxLevelReachable = args.currentPlayer->getDificultyOfCard(CardsEnum::economy); // TODO: check if the player can reach a higher level depending on his caracteristics
    for (auto &element : caravanMovementPath)
    {
        if ((*map)(element[0], element[1])->getFieldLevel() > (FieldLevel)maxLevelReachable)
        {
            return false;
        }
    }

    std::vector<std::shared_ptr<Caravan>> caravans = args.currentPlayer->getCaravans();

    switch (cardLevel)
    {
    case 1:
        if (!verifyIfPathSizeIsCorrect(caravanMovementPath.size(), CARAVAN_STEPS_AT_LEVEL_1 + numberOfBoxUsed))
        {
            return false;
        }
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, CARVAN_NUMBER_AT_LEVEL_1);

        moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        break;
    case 2:
        if (!verifyIfPathSizeIsCorrect(caravanMovementPath.size(), CARAVAN_STEPS_AT_LEVEL_2 + numberOfBoxUsed))
        {
            return false;
        }
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, CARVAN_NUMBER_AT_LEVEL_2);
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
        if (!verifyIfPathSizeIsCorrect(caravanMovementPath.size(), CARAVAN_STEPS_AT_LEVEL_3 + numberOfBoxUsed))
        {
            return false;
        }
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, CARVAN_NUMBER_AT_LEVEL_3);
        moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // exchangeRessourceWithOtherPlayer(); //  TODO: allow to exchange ressource with other player
        break;
    case 4:
        if (!verifyIfPathSizeIsCorrect(caravanMovementPath.size(), CARAVAN_STEPS_AT_LEVEL_4 + numberOfBoxUsed))
        {
            return false;
        }
        addCaravanAfterCardAmelioration(caravans, args.currentPlayer, CARVAN_NUMBER_AT_LEVEL_4);
        moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // moveCaravan(caravans, caravanMovementPath.at(0), caravanMovementPath.at(caravanMovementPath.size() - 1), map, args.currentPlayer);
        // playAnotherCard(); // TODO: allow to play another card
        break;
    default:
        return false;
    }

    roundCards(args.currentPlayer, args.ruleId);
    return true;
}

/**
 * @file Rules.cpp
 * @fn bool Rules::verifyIfPathSizeIsCorrect(int pathSize, int maxPathSize)
 * @brief This function aim to verify if the path size is correct
 * @param pathSize the size of the path
 * @param maxPathSize the max size of the path
 */
bool Rules::verifyIfPathSizeIsCorrect(int pathSize, int maxPathSize)
{
    if (pathSize > maxPathSize)
    {
        return false;
    }
    return true;
}

/**
 * @file Rules.cpp
 * @fn void Rules::addCaravanAfterCardAmelioration(std::vector<std::shared_ptr<Caravan>> &caravans, std::shared_ptr<Player> currentPlayer, int numberOfCaravans)
 * @brief This function aim to add caravan after the card amelioration
 * @param caravans the caravans of the player
 * @param currentPlayer the current player
 * @param numberOfCaravans the number of caravans related to the card level
 */
void Rules::addCaravanAfterCardAmelioration(std::vector<std::shared_ptr<Caravan>> &caravans, std::shared_ptr<Player> currentPlayer, int numberOfCaravans)
{
    while (caravans.size() < (std::size_t)numberOfCaravans)
    {
        std::array<unsigned, 2> pos = {0, 0};
        std::shared_ptr<Caravan> caravan = std::make_shared<Caravan>(pos, currentPlayer->getName());
        currentPlayer->addCaravan(caravan);
        caravans.push_back(caravan);
    }
}

/**
 * @file Rules.cpp
 * @fn bool Rules::moveCaravan(std::vector<std::shared_ptr<Caravan>> caravans, std::array<unsigned, 2> pos1, std::array<unsigned, 2> pos2, std::shared_ptr<Map> map, std::shared_ptr<Player> currentPlayer)
 * @brief This function aim to move the caravan
 * @param caravans the caravans of the player
 * @param pos1 the first position of the caravan
 * @param pos2 the second position of the caravan
 * @param map the map
 *
 */
bool Rules::moveCaravan(std::vector<std::shared_ptr<Caravan>> caravans, std::array<unsigned, 2> pos1, std::array<unsigned, 2> pos2, std::shared_ptr<Map> map, std::shared_ptr<Player> currentPlayer)
{
    for (auto caravan : caravans)
    {
        if (caravan->getPosition() == pos1 && caravan->isUsed() == true)
        {
            caravan->setPos(pos2);
            (*map)(pos1[0], pos1[1])->removeElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
            (*map)(pos2[0], pos2[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
            return true;
        }
    }

    if (!(isThereACity(pos1, map) || isThereAControlPawn(pos1, map)))
    {
        return false;
    }
    for (auto caravan : caravans)
    {
        if (!(caravan->isUsed()))
        {
            caravan->setPos(pos2);
            caravan->setUsed(true);
            (*map)(pos2[0], pos2[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*caravan));
            return true;
        }
    }
    return true;
}

/**
 * @file Rules.cpp
 * @fn elementList Rules::checkIfBarbarianIsOnThePath(std::vector<std::array<unsigned, 2>> caravanMovementPath, std::shared_ptr<shared::Map> map)
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
 * @fn bool Rules::checkIfWaterIsOnThePath(std::vector<std::array<unsigned, 2>> caravanMovementPath, std::shared_ptr<shared::Map> map)
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
 * @fn bool Rules::playScienceCard(RuleArgsStruct &args)
 * @brief This function aim to run the rule of the science card
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::playScienceCard(RuleArgsStruct &args)
{
    std::shared_ptr<Player> currentPlayer = args.currentPlayer;
    unsigned numberOfBoxUsed = args.numberOfBoxUsed;
    if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::science))
    {
        return false;
    }
    args.currentPlayer->deleteBox(CardsEnum::science, numberOfBoxUsed);

    CardsEnum cardToGetABox;
    ResourceEnum resourceToGet;
    std::array<unsigned, 2> positionToNuke;
    std::shared_ptr<Map> gameMap;
    std::vector<CardsEnum> cardsToImprove = args.cardsToImprove;

    std::vector<std::array<unsigned, 2>> neighbors;
    unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::science);
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
            return false;
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
        return false;
    }

    unsigned dificulty = currentPlayer->getDificultyOfCard(CardsEnum::science);
    std::array<int, 3> cardsLevelToImprove = currentPlayer->incrementTechWheel(dificulty + numberOfBoxUsed);
    for (auto cardtoImprove : cardsToImprove)
    {
        if (cardsLevelToImprove[currentPlayer->getLevelOfCard(cardtoImprove) - 1] > 0)
        {
            cardsLevelToImprove[currentPlayer->getLevelOfCard(cardtoImprove) - 1]--;
            args.currentPlayer->upgradeCard(cardtoImprove);
        }
    }

    roundCards(args.currentPlayer, args.ruleId);
    return true;
}

void Rules::killControlPawn(std::shared_ptr<Map> gameMap, std::array<unsigned, 2> position, std::shared_ptr<Player> currentPlayer)
{
    elementList elements = (*gameMap)(position[0], position[1])->getElements();
    for (auto element : elements)
    {
        if (std::holds_alternative<ControlPawn>(*element))
        {
            if (std::get<ControlPawn>(*element).isReinforced())
            {
                std::get<ControlPawn>(*element).setNotReinforced();
            }
            else
            {
                (*gameMap)(position[0], position[1])->removeElement(element);
                currentPlayer->removeControlPawn(std::make_shared<ControlPawn>(std::get<ControlPawn>(*element)));
            }
        }
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
        killControlPawn(gameMap, neightbor, currentPlayer);
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
    std::vector<std::array<unsigned, 2>> neighbors;
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
            neighbors.push_back({posX - 1, posY - 1}); // faux
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
        if (posX > 0)
        {
            neighbors.push_back({posX - 1, posY});
        }
        if (posY < mapSizeY - 1)
        {
            neighbors.push_back({posX, posY + 1});
        }
        if (posY < mapSizeY - 1 && posX < mapSizeX - 1)
        {
            neighbors.push_back({posX + 1, posY + 1});
        }
        if (posX < mapSizeX - 1)
        {
            neighbors.push_back({posX + 1, posY});
        }
    }
    return neighbors;
}

/// CULTURE CARD ///
/**
 * @file Rules.cpp
 * @fn bool Rules::playCultureCard(RuleArgsStruct &args)
 * @brief This function aim to play the culture card
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::playCultureCard(RuleArgsStruct &args)
{
    unsigned numberOfBoxUsed = args.numberOfBoxUsed;
    if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::culture))
    {
        return false;
    }

    std::vector<std::array<unsigned, 2>> pawnsPositions = args.pawnsPositions;
    std::shared_ptr<Map> gameMap = args.gameMap;

    unsigned cardLevel = args.currentPlayer->getLevelOfCard(CardsEnum::culture);
    switch (cardLevel)
    {
    case 1:
        if (pawnsPositions.size() <= NUMBER_PAWNS_ECONOMY_LVL_1 + numberOfBoxUsed)
        {
            if (!placeControlPawns(pawnsPositions, gameMap, args.currentPlayer))
            {
                return false;
            }
        }
        else
        {
            return false;
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
        return false;
    }

    roundCards(args.currentPlayer, args.ruleId);
    return true;
}

/**
 * @file Rules.cpp
 * @fn bool Rules::placeControlPawns(std::vector<std::array<unsigned, 2>> positions, std::shared_ptr<Map> gameMap, std::shared_ptr<Player> currentPlayer)
 * @brief This function aim to place the control pawns on the map
 * @param positions the positions where the control pawns will be placed
 * @param gameMap the map of the game
 * @param currentPlayer the current player that owns the control pawns
 */
bool Rules::placeControlPawns(std::vector<std::array<unsigned, 2>> positions, std::shared_ptr<Map> gameMap, std::shared_ptr<Player> currentPlayer) // TODO : split this function
{
    for (auto position : positions)
    {
        for (auto element : (*gameMap)(position[0], position[1])->getElements())
        {
            if (!std::holds_alternative<Caravan>(*element))
            {
                return false;
            }
        }
        if (isThereACityAround(position, gameMap))
        {
            std::shared_ptr<shared::ControlPawn> controlPawn = std::make_shared<shared::ControlPawn>(position, currentPlayer->getName());
            currentPlayer->addControlPawn(controlPawn);
            std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> element = *controlPawn;
            (*gameMap)(position[0], position[1])->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));
        }
    }
    return true;
}

/**
 * @file Rules.cpp
 * @fn bool Rules::isThereACityAround(std::array<unsigned, 2> position, std::shared_ptr<Map> gameMap)
 * @brief This function aim to check if there is a city around a position
 * @param position the position to check
 * @param gameMap the map of the game
 * @return true if there is a city around, false otherwise
 */
bool Rules::isThereACityAround(std::array<unsigned, 2> position, std::shared_ptr<Map> gameMap)
{
    std::vector<std::array<unsigned, 2>> neighbors = getNeighbors(position[0], position[1], gameMap);
    for (auto neighbor : neighbors)
    {
        for (auto element : (*gameMap)(neighbor[0], neighbor[1])->getElements())
        {
            if (std::holds_alternative<City>(*element))
            {
                return true; // TODO : check if the city is owned by the player
            }
        }
    }
    return false;
}

bool Rules::isThereACity(std::array<unsigned, 2> position, std::shared_ptr<Map> gameMap)
{
    for (auto element : (*gameMap)(position[0], position[1])->getElements())
    {
        if (std::holds_alternative<City>(*element))
        {
            return true; // TODO : check if the city is owned by the player
        }
    }
    return false;
}

/**
 * @file Rules.cpp
 * @fn bool Rules::isThereAControlPawnAround(std::array<unsigned, 2> position, std::shared_ptr<Map> gameMap)
 * @brief This function aim to check if there is a control pawn around a position
 * @param position the position to check
 * @param gameMap the map of the game
 * @return true if there is a control pawn around, false otherwise
 */
bool Rules::isThereAControlPawn(std::array<unsigned, 2> position, std::shared_ptr<Map> gameMap)
{
    for (auto element : (*gameMap)(position[0], position[1])->getElements())
    {
        if (std::holds_alternative<ControlPawn>(*element))
        {
            return true; // TODO : check if the controlpawn is owned by the player
        }
    }

    return false;
}

/// MILITARY CARD ///
/**
 * @file Rules.cpp
 * @fn bool Rules::playMilitaryCard(RuleArgsStruct &args)
 * @brief This function aim to play the military card
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::playMilitaryCard(RuleArgsStruct &args)
{
    if (args.militaryCardAttack == true)
    {
        return attack(args);
    }
    else
    {
        return reinforce(args);
    }
}

/**
 * @file Rules.cpp
 * @fn bool Rules::attack(RuleArgsStruct &args)
 * @brief This function aim to attack an enemy pawn or a city
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::attack(RuleArgsStruct &args)
{
    // TODO : attack
    // I have problems mesuring the distance of the enemy pawn from the current player hexagons
    return false;
}

/**
 * @file Rules.cpp
 * @fn bool Rules::reinforce(RuleArgsStruct &args)
 * @brief This function aim to reinforce a pawn
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::reinforce(RuleArgsStruct &args)
{
    unsigned numberOfBoxUsed = args.numberOfBoxUsed;
    if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::military))
    {
        return false;
    }

    std::shared_ptr<Map> gameMap = args.gameMap;
    std::vector<std::array<unsigned, 2>> pawnsPositions = args.pawnsPositions;
    unsigned cardDifficulty = args.currentPlayer->getDificultyOfCard(CardsEnum::military);
    if (pawnsPositions.size() > cardDifficulty + numberOfBoxUsed)
    {
        return false;
    }
    std::shared_ptr<ControlPawn> controlPawn;

    for (auto position : pawnsPositions)
    {
        for (auto element : (*gameMap)(position[0], position[1])->getElements())
        {
            if (std::holds_alternative<ControlPawn>(*element))
            {
                std::get<ControlPawn>(*element).setReinforced();
                break;
            }
        }
    }

    roundCards(args.currentPlayer, args.ruleId);
    return true;
}

/// INDUSTRY CARD ///
/**
 * @file Rules.cpp
 * @fn bool Rules::playIndustryCard(RuleArgsStruct &args)
 * @brief This function aim to play the industry card
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::playIndustryCard(RuleArgsStruct &args)
{
    if (args.industryCardBuildWonder == true)
    {
        return buildWonder(args);
    }
    else
    {
        return buildCity(args);
    }
}

/**
 * @file Rules.cpp
 * @fn bool Rules::buildWonder(RuleArgsStruct &args)
 * @brief This function aim to build a wonder
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::buildWonder(RuleArgsStruct &args) // TODO : remake this function
{
    // TODO : build a wonder
    return false;
}

/**
 * @file Rules.cpp
 * @fn bool Rules::buildCity(RuleArgsStruct &args)
 * @brief This function aim to build a city
 * @param args : struct containing all the arguments needed to run the rule
 */
bool Rules::buildCity(RuleArgsStruct &args) // TODO : Check the distance to controlled hexagon
{
    unsigned numberOfBoxUsed = args.numberOfBoxUsed;
    if (numberOfBoxUsed > args.currentPlayer->getNumberOfBox(CardsEnum::industry))
    {
        return false;
    }

    std::shared_ptr<Map> gameMap = args.gameMap;
    std::array<unsigned, 2> position = args.positionOfCity;

    unsigned cardDifficulty = args.currentPlayer->getDificultyOfCard(CardsEnum::industry);
    if (cardDifficulty + numberOfBoxUsed < (unsigned)(*gameMap)(position[0], position[1])->getFieldLevel())
    {
        return false;
    }

    for (auto element : (*gameMap)(position[0], position[1])->getElements())
    {
        if (!(std::holds_alternative<Caravan>(*element)))
        {
            return false;
        }
    }
    std::shared_ptr<City> city = std::make_shared<City>(position, args.currentPlayer->getName());
    args.currentPlayer->addCity(city);
    (*gameMap)(position[0], position[1])->addElement(std::make_shared<std::variant<Caravan, Barbarian, BarbarianVillage, ControlPawn, City>>(*city));

    roundCards(args.currentPlayer, args.ruleId);
    return true;
}

void Rules::roundCards(std::shared_ptr<Player> currentPlayer, CardsEnum cardPlayed)
{
    unsigned initialDificulty = currentPlayer->getDificultyOfCard(cardPlayed);
    for (auto card : currentPlayer->getListOfPriorityCards())
    {
        if (card->getType() == cardPlayed)
        {
            card->setDificulty(1);
        }
        else if (card->getDificulty() < initialDificulty)
        {
            card->setDificulty(card->getDificulty() + 1);
        }
    }
}

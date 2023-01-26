#include <shared.hpp>
#include <iostream>
#include <variant>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( RulesTest)

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestRulesScienceCardLevel1)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    int initialDifficulty = player->getDificultyOfCard(shared::CardsEnum::science);
    player->addBox(shared::CardsEnum::science, 50);
    shared::Rules rules;
    shared::RuleArgsStruct args;
    args.ruleId = shared::CardsEnum::science;
    args.numberOfBoxUsed = 19;
    args.currentPlayer = player;

    rules.runTheRule(args);
    BOOST_CHECK(player->getTechLevel() == 22);
}

BOOST_AUTO_TEST_CASE(TestRulesScienceCardLevel2)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    player->addBox(shared::CardsEnum::science, 50);
    shared::Rules rules;
    shared::RuleArgsStruct args;
    args.ruleId = shared::CardsEnum::science;
    args.numberOfBoxUsed = 19;
    args.currentPlayer = player;
    args.cardsToImprove.push_back(shared::CardsEnum::science);

    rules.runTheRule(args);
    BOOST_CHECK(player->getLevelOfCard(shared::CardsEnum::science) == 2);

    args.numberOfBoxUsed = 0;
    args.cardToGetABox = shared::CardsEnum::economy;
    BOOST_CHECK(player->getNumberOfBox(shared::CardsEnum::economy) == 0);
    rules.runTheRule(args);
    BOOST_CHECK(player->getNumberOfBox(shared::CardsEnum::economy) == 1);
}

BOOST_AUTO_TEST_CASE(TestRulesScienceCardLevel3)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    player->addBox(shared::CardsEnum::science, 50);
    shared::Rules rules;
    shared::RuleArgsStruct args;
    args.ruleId = shared::CardsEnum::science;
    args.numberOfBoxUsed = 19;
    args.currentPlayer = player;
    args.cardsToImprove.push_back(shared::CardsEnum::science);
    args.cardsToImprove.push_back(shared::CardsEnum::science);

    rules.runTheRule(args);
    BOOST_CHECK(player->getLevelOfCard(shared::CardsEnum::science) == 3);

    args.numberOfBoxUsed = 0;
    args.resourceToGet = shared::ResourceEnum::antiquity;
    BOOST_CHECK(player->getNumberOfResource(shared::ResourceEnum::antiquity) == 0);
    rules.runTheRule(args);
    BOOST_CHECK(player->getNumberOfResource(shared::ResourceEnum::antiquity) == 1);
}

BOOST_AUTO_TEST_CASE(TestRulesScienceCardLevel4)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    player->addBox(shared::CardsEnum::science, 50);
    shared::Rules rules;
    shared::RuleArgsStruct args;
    args.ruleId = shared::CardsEnum::science;
    args.numberOfBoxUsed = 30;
    args.currentPlayer = player;
    args.cardsToImprove.push_back(shared::CardsEnum::science);
    args.cardsToImprove.push_back(shared::CardsEnum::science);
    args.cardsToImprove.push_back(shared::CardsEnum::science);
    rules.runTheRule(args);

    // std::cout << "Level of science card: " << player->getLevelOfCard(shared::CardsEnum::science) << std::endl;
    BOOST_CHECK(player->getLevelOfCard(shared::CardsEnum::science) == 4);
    std::shared_ptr<shared::Map> map = std::make_shared<shared::Map>(10, 10);
    map->init();

    std::array<unsigned, 2> position = {5, 5};
    std::shared_ptr<shared::ControlPawn> controlPawn = std::make_shared<shared::ControlPawn>(position, player->getName());
    player->addControlPawn(controlPawn);
    std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> element = *controlPawn;
    (*map)(5, 5)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    position = {4, 4};
    controlPawn = std::make_shared<shared::ControlPawn>(position, player->getName());
    player->addControlPawn(controlPawn);
    element = *controlPawn;
    (*map)(4, 4)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    position = {6, 6};
    controlPawn = std::make_shared<shared::ControlPawn>(position, player->getName());
    player->addControlPawn(controlPawn);
    element = *controlPawn;
    (*map)(6, 6)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    args.numberOfBoxUsed = 0;
    args.gameMap = map;
    args.positionToNuke = {5, 5};
    args.cardsToImprove.clear();
    BOOST_CHECK((*map)(5, 5)->getElements().size() == 1);
    BOOST_CHECK((*map)(4, 4)->getElements().size() == 1);
    BOOST_CHECK((*map)(6, 6)->getElements().size() == 1);
    rules.runTheRule(args);
    BOOST_CHECK((*map)(5, 5)->getElements().size() == 0);
    BOOST_CHECK((*map)(4, 4)->getElements().size() == 0);
    BOOST_CHECK((*map)(6, 6)->getElements().size() == 1);
}

BOOST_AUTO_TEST_CASE(TestRulesEconomyCardLevel1)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    shared::Rules rules;
    shared::RuleArgsStruct args;
    std::shared_ptr<shared::Map> map = std::make_shared<shared::Map>(10, 10);
    map->init();

    std::array<unsigned, 2> position = {0, 1};
    std::shared_ptr<shared::City> city = std::make_shared<shared::City>(position, player->getName());
    player->addCity(city);
    std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> element = *city;
    (*map)(0, 1)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    args.ruleId = shared::CardsEnum::economy;
    args.gameMap = map;
    args.numberOfBoxUsed = 0;
    args.currentPlayer = player;
    args.caravanMovementPath.push_back({0, 0});
    args.caravanMovementPath.push_back({1, 1});
    args.caravanMovementPath.push_back({2, 1});
    rules.runTheRule(args);

    BOOST_CHECK((*map)(0, 0)->getElements().size() == 0);
    BOOST_CHECK((*map)(2, 1)->getElements().size() == 1);
    BOOST_CHECK(std::holds_alternative<shared::Caravan>(*((*map)(2, 1)->getElements().at(0))) == true);
}

BOOST_AUTO_TEST_CASE(TestRulesMilitaryReinforceAllLevels)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    shared::Rules rules;
    shared::RuleArgsStruct args;
    std::shared_ptr<shared::Map> map = std::make_shared<shared::Map>(10, 10);
    map->init();

    
    player->addBox(shared::CardsEnum::military, 50);
    

    args.ruleId = shared::CardsEnum::military;
    args.militaryCardAttack = false;
    args.currentPlayer = player;
    args.gameMap = map;
    args.numberOfBoxUsed = 1;
    args.pawnsPositions.push_back({2, 2});
    args.pawnsPositions.push_back({3, 4});

    std::array<unsigned, 2> position = {2, 2};
    std::shared_ptr<shared::ControlPawn> controlPawn = std::make_shared<shared::ControlPawn>(position, player->getName());
    player->addControlPawn(controlPawn);
    std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> element = *controlPawn;
    (*map)(2, 2)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    position = {3, 4};
    controlPawn = std::make_shared<shared::ControlPawn>(position, player->getName());
    player->addControlPawn(controlPawn);
    element = *controlPawn;
    (*map)(3, 4)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    position = {5, 5};
    controlPawn = std::make_shared<shared::ControlPawn>(position, player->getName());
    player->addControlPawn(controlPawn);
    element = *controlPawn;
    (*map)(5, 5)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    rules.runTheRule(args);

    BOOST_CHECK((*map)(2, 2)->getElements().size() == 1);
    BOOST_CHECK((*map)(3, 4)->getElements().size() == 1);
    BOOST_CHECK(std::get<shared::ControlPawn>(*((*map)(2, 2)->getElements().at(0))).isReinforced());
    BOOST_CHECK(std::get<shared::ControlPawn>(*((*map)(3, 4)->getElements().at(0))).isReinforced());
    BOOST_CHECK(std::get<shared::ControlPawn>(*((*map)(5, 5)->getElements().at(0))).isReinforced() == false);
}

BOOST_AUTO_TEST_CASE(TestRulesIndustryCardForBuildingCity)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    player->addBox(shared::CardsEnum::industry, 50);
    shared::Rules rules;
    shared::RuleArgsStruct args;
    std::shared_ptr<shared::Map> map = std::make_shared<shared::Map>(10, 10);
    map->init();
    args.gameMap = map;
    args.ruleId = shared::CardsEnum::industry;
    args.industryCardBuildWonder = false;
    args.numberOfBoxUsed = 0;
    args.currentPlayer = player;
    args.positionOfCity = {7, 8};

    rules.runTheRule(args);

    BOOST_CHECK((*map)(7, 8)->getElements().size() == 1);
    BOOST_CHECK(std::holds_alternative<shared::City>(*((*map)(7, 8)->getElements().at(0))));
    BOOST_CHECK(player->getCityList().at(0)->getPosition()[0] == 7 && player->getCityList().at(0)->getPosition()[1] == 8);
}

BOOST_AUTO_TEST_CASE(TestRulesCultureCardLevel1)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    player->addBox(shared::CardsEnum::culture, 50);
    shared::Rules rules;
    shared::RuleArgsStruct args;
    std::shared_ptr<shared::Map> map = std::make_shared<shared::Map>(10, 10);
    map->init();

    std::array<unsigned, 2> position = {4, 3};
    std::shared_ptr<shared::City> city = std::make_shared<shared::City>(position, player->getName());
    player->addCity(city);
    std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> element = *city;
    (*map)(position[0], position[1])->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    args.ruleId = shared::CardsEnum::culture;
    args.gameMap = map;
    args.numberOfBoxUsed = 1;
    args.currentPlayer = player;
    args.pawnsPositions.push_back({3, 2});
    args.pawnsPositions.push_back({5, 3});
    rules.runTheRule(args);

    BOOST_CHECK((*map)(3, 2)->getElements().size() == 1);
    BOOST_CHECK((*map)(5, 3)->getElements().size() == 1);
    BOOST_CHECK(std::holds_alternative<shared::ControlPawn>(*((*map)(3, 2)->getElements().at(0))));
    BOOST_CHECK(std::holds_alternative<shared::ControlPawn>(*((*map)(5, 3)->getElements().at(0))));

}

BOOST_AUTO_TEST_SUITE_END()

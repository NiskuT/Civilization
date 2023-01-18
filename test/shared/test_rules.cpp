#include <shared.hpp>
#include <iostream>
#include <variant>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MapTest)

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestRulesScienceCardLevel1)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    player->addBox(shared::CardsEnum::science, 50);
    shared::Rules rules;
    shared::RuleArgsStruct args;
    args.ruleId = shared::CardsEnum::science;
    args.numberOfBoxUsed = 19;
    args.currentPlayer = player;

    rules.runTheRule(args);
    BOOST_CHECK(player->getTechLevel() == 20);
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
    std::cout << "TestRulesScienceCardLevel4" << std::endl;
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

    BOOST_CHECK(player->getLevelOfCard(shared::CardsEnum::science) == 4);
    std::shared_ptr<shared::Map> map(new shared::Map(10, 10));

    std::shared_ptr<shared::ControlPawn> controlPawn = std::make_shared<shared::ControlPawn>();
    player->addControlPawn(controlPawn);
    std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> element = *controlPawn;
    (*map)(5, 5)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    controlPawn = std::make_shared<shared::ControlPawn>();
    player->addControlPawn(controlPawn);
    element = *controlPawn;
    (*map)(4,4)->addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element));

    controlPawn = std::make_shared<shared::ControlPawn>();
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

BOOST_AUTO_TEST_SUITE_END()

#include <shared.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MapTest)

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestRulesScienceCardImproveScienceLevel)
{
    std::shared_ptr<shared::Player> player = std::make_shared<shared::Player>();
    shared::Rules rules;
    shared::RuleArgsStruct args;
    args.ruleId = shared::CardsEnum::science;
    args.numberOfBoxUsed = 2;
    args.currentPlayer = player;
    args.cardsToImprove.push_back(shared::CardsEnum::science);

    rules.runTheRule(args);
    BOOST_CHECK(player->getTechLevel() == 3);
    BOOST_CHECK(player->getLevelOfCard(shared::CardsEnum::science) == 2);
}

BOOST_AUTO_TEST_SUITE_END()

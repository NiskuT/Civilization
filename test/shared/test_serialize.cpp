#include <shared.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(SerializeTest)

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(serializeRuleArgsStruct)
{
    shared::RuleArgsStruct ruleArgsStruct;
    shared::RuleArgsStruct ruleArgsStruct2;

    ruleArgsStruct.ruleId = shared::CardsEnum::economy;
    ruleArgsStruct.numberOfBoxUsed = 2;
    ruleArgsStruct.caravanMovementPath.push_back({0, 1});
    ruleArgsStruct.caravanMovementPath.push_back({4, 8});
    ruleArgsStruct.caravanMovementPath.push_back({5, 6});
    ruleArgsStruct.resourceToGet = shared::ResourceEnum::oil;
    ruleArgsStruct.cardToGetABox = shared::CardsEnum::economy;
    ruleArgsStruct.positionToNuke = {3, 4};
    ruleArgsStruct.pawnsPositions.push_back({1, 1});
    ruleArgsStruct.pawnsPositions.push_back({2, 2});
    ruleArgsStruct.pawnsPositions.push_back({3, 3});
    ruleArgsStruct.militaryCardAttack = true;
    ruleArgsStruct.industryCardBuildWonder = true;
    ruleArgsStruct.positionOfWonder = {4, 5};
    ruleArgsStruct.positionOfCity = {6, 7};
    ruleArgsStruct.cardsToImprove.push_back(shared::CardsEnum::economy);
    ruleArgsStruct.cardsToImprove.push_back(shared::CardsEnum::military);

    shared::Binary binary;
    std::string struc;
    binary.castToBinary(ruleArgsStruct, struc);
    binary.castToObject(struc, ruleArgsStruct2);

    BOOST_CHECK(ruleArgsStruct.ruleId == ruleArgsStruct2.ruleId);
    BOOST_CHECK(ruleArgsStruct.numberOfBoxUsed == ruleArgsStruct2.numberOfBoxUsed);
    BOOST_CHECK(ruleArgsStruct.caravanMovementPath == ruleArgsStruct2.caravanMovementPath);
    BOOST_CHECK(ruleArgsStruct.resourceToGet == ruleArgsStruct2.resourceToGet);
    BOOST_CHECK(ruleArgsStruct.cardToGetABox == ruleArgsStruct2.cardToGetABox);
    BOOST_CHECK(ruleArgsStruct.positionToNuke == ruleArgsStruct2.positionToNuke);
    BOOST_CHECK(ruleArgsStruct.pawnsPositions == ruleArgsStruct2.pawnsPositions);
    BOOST_CHECK(ruleArgsStruct.militaryCardAttack == ruleArgsStruct2.militaryCardAttack);
    BOOST_CHECK(ruleArgsStruct.industryCardBuildWonder == ruleArgsStruct2.industryCardBuildWonder);
    BOOST_CHECK(ruleArgsStruct.positionOfWonder == ruleArgsStruct2.positionOfWonder);
    BOOST_CHECK(ruleArgsStruct.positionOfCity == ruleArgsStruct2.positionOfCity);
    BOOST_CHECK(ruleArgsStruct.cardsToImprove == ruleArgsStruct2.cardsToImprove);

}

BOOST_AUTO_TEST_SUITE_END()

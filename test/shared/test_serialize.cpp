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

BOOST_AUTO_TEST_CASE(Barbarian)
{
    shared::Barbarian barbarian;
    shared::Binary binary;
    std::string barbarianSerialized;
    binary.castToBinary(barbarian, barbarianSerialized);

    std::shared_ptr<shared::Barbarian> barbarianDeserialized = std::make_shared<shared::Barbarian>();
    binary.castToObject(barbarianSerialized, *barbarianDeserialized);

    BOOST_CHECK(barbarianDeserialized->isAlive == barbarian.isAlive);
}

BOOST_AUTO_TEST_CASE(BarbarianVillage)
{
    shared::BarbarianVillage barbarianVillage;
    shared::Binary binary;
    std::string barbarianVillageSerialized;
    binary.castToBinary(barbarianVillage, barbarianVillageSerialized);

    std::shared_ptr<shared::BarbarianVillage> barbarianVillageDeserialized = std::make_shared<shared::BarbarianVillage>();
    binary.castToObject(barbarianVillageSerialized, *barbarianVillageDeserialized);
}

BOOST_AUTO_TEST_CASE(Caravan)
{
    shared::Caravan caravan({2, 4}, "not defined");
    caravan.setUsed(true);
    caravan.player = "player";
    shared::Binary binary;
    std::string caravanSerialized;
    binary.castToBinary(caravan, caravanSerialized);

    std::shared_ptr<shared::Caravan> caravanDeserialized = std::make_shared<shared::Caravan>();
    caravanDeserialized->setPos({2, 4});
    binary.castToObject(caravanSerialized, *caravanDeserialized);

    BOOST_CHECK(caravanDeserialized->isUsed() == caravan.isUsed());
    BOOST_CHECK(caravanDeserialized->player == caravan.player);
    BOOST_CHECK(caravanDeserialized->getPosition() == caravan.getPosition());
}

BOOST_AUTO_TEST_CASE(City)
{
    shared::City city({2, 4}, "not defined");
    city.setStateCity(shared::CityStateEnum::seoul);
    city.player = "player";
    shared::Binary binary;
    std::string citySerialized;
    binary.castToBinary(city, citySerialized);

    std::shared_ptr<shared::City> cityDeserialized = std::make_shared<shared::City>();
    binary.castToObject(citySerialized, *cityDeserialized);

    BOOST_CHECK(cityDeserialized->getPosition() == city.getPosition());
    BOOST_CHECK(cityDeserialized->player == city.player);
    BOOST_CHECK(cityDeserialized->isStateCity == city.isStateCity);
}

BOOST_AUTO_TEST_CASE(ControlPawn)
{
    shared::ControlPawn controlPawn({2, 4}, "not defined");
    controlPawn.setReinforced();
    controlPawn.player = "player";
    shared::Binary binary;
    std::string controlPawnSerialized;
    binary.castToBinary(controlPawn, controlPawnSerialized);

    std::shared_ptr<shared::ControlPawn> controlPawnDeserialized = std::make_shared<shared::ControlPawn>();
    binary.castToObject(controlPawnSerialized, *controlPawnDeserialized);

    BOOST_CHECK(controlPawnDeserialized->getPosition() == controlPawn.getPosition());
    BOOST_CHECK(controlPawnDeserialized->isReinforced() == controlPawn.isReinforced());
    BOOST_CHECK(controlPawnDeserialized->player == controlPawn.player);
}

BOOST_AUTO_TEST_CASE(Hexagon)
{
    shared::Hexagon hexagon;
    hexagon.setFieldType(shared::FieldLevel::Mountain);
    shared::ControlPawn controlPawn({2, 4}, "not defined");
    controlPawn.setReinforced();
    controlPawn.player = "player";
    std::shared_ptr<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>> elem;
    elem = std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(controlPawn);
    hexagon.addElement(elem);

    shared::Binary binary;
    std::string hexagonSerialized;
    binary.castToBinary(hexagon, hexagonSerialized);

    std::shared_ptr<shared::Hexagon> hexagonDeserialized = std::make_shared<shared::Hexagon>();
    binary.castToObject(hexagonSerialized, *hexagonDeserialized);

    BOOST_CHECK(hexagonDeserialized->getFieldLevel() == hexagon.getFieldLevel());
    BOOST_CHECK(hexagonDeserialized->getElements().size() == hexagon.getElements().size());
    std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> elemDeserialized = *(hexagonDeserialized->getElements()[0]);
    shared::ControlPawn controlPawnDeserialized = std::get<shared::ControlPawn>(*(hexagonDeserialized->getElements()[0]));
    BOOST_CHECK(controlPawnDeserialized.getPosition() == controlPawn.getPosition());
    BOOST_CHECK(controlPawnDeserialized.isReinforced() == controlPawn.isReinforced());
    BOOST_CHECK(controlPawnDeserialized.player == controlPawn.player);
}

BOOST_AUTO_TEST_CASE(test_Map_Serialization)
{
    shared::Map map(10, 10);
    map.generateRandomMap(123);

    shared::Binary binary;
    std::string mapSerialized;
    binary.castToBinary(map, mapSerialized);

    shared::Map mapDeserialized;
    binary.castToObject(mapSerialized, mapDeserialized);

    BOOST_CHECK_EQUAL(map.getMapWidth(), mapDeserialized.getMapWidth());
    BOOST_CHECK_EQUAL(map.getMapHeight(), mapDeserialized.getMapHeight());

    for (unsigned i = 0; i < map.getMapHeight(); i++)
    {
        for (unsigned j = 0; j < map.getMapWidth(); j++)
        {
            BOOST_CHECK(map(i, j)->getFieldLevel() == mapDeserialized(i, j)->getFieldLevel());
            BOOST_CHECK_EQUAL(map(i, j)->getElements().size(), mapDeserialized(i, j)->getElements().size());
        }
    }
}
BOOST_AUTO_TEST_SUITE_END()

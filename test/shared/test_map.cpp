#include <shared.hpp>
#include <ctime>
#include <iostream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( MapTest )

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( constructorAndGetters )
{
    shared::Map map(20,15);

    BOOST_CHECK(map.getMapWidth() == 20);
    BOOST_CHECK(map.getMapHeight() == 15);

    map.setMapHeight(23);
    map.setMapWidth(12);

    BOOST_CHECK(map.getMapWidth() == 12);
    BOOST_CHECK(map.getMapHeight() == 23);
}

BOOST_AUTO_TEST_CASE( operatorParentheses )
{
    shared::Map map(20,15);
    map.init();

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 15; j++)  BOOST_CHECK(map(i,j) != nullptr);
    }
}

BOOST_AUTO_TEST_CASE( operatorParenthesesOutOfRange )
{
    shared::Map map(20,15);

    BOOST_CHECK(map(20,15) == nullptr);
    BOOST_CHECK(map(20,0) == nullptr);
    BOOST_CHECK(map(0,15) == nullptr);
    BOOST_CHECK(map(-1,0) == nullptr);
    BOOST_CHECK(map(0,-1) == nullptr);
}

BOOST_AUTO_TEST_CASE( RadomMap )
{
    shared::Map map(20,15);
    
    srand(time(NULL));
    int seed = rand() % 1000000;
    map.generateRandomMap(seed);

    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 20; j++) BOOST_CHECK(int(map(j,i)->getFieldLevel()) >= 0 && int(map(j,i)->getFieldLevel()) <= 11);
    }
}

BOOST_AUTO_TEST_SUITE_END()
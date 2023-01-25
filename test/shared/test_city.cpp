#include <shared.hpp>
#include <ctime>
#include <iostream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( CityTest )

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( constructorAndGetters )
{
    shared::City city({20, 15});

    BOOST_CHECK(city.getPosition()[0] == 20);
    BOOST_CHECK(city.getPosition()[1] == 15);

    shared::ElementEnum type = city.getType();
    BOOST_CHECK(type == shared::ElementEnum::city);

    BOOST_CHECK(city.isMature == false);
    BOOST_CHECK(city.isCapital == false);
    BOOST_CHECK(city.isStateCity == false);
}

BOOST_AUTO_TEST_CASE( setStateCity )
{
    shared::City city({0, 0});
    city.setStateCity(shared::CityStateEnum::mohenjoDaro);

    BOOST_CHECK(city.stateCityType == shared::CityStateEnum::mohenjoDaro);
    BOOST_CHECK(city.isStateCity == true);

}

BOOST_AUTO_TEST_SUITE_END()

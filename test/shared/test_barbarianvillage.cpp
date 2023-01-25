#include <shared.hpp>
#include <ctime>
#include <iostream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( BarbarianVillageTest )

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( constructorAndGetters )
{
    shared::BarbarianVillage village;

    shared::ElementEnum type = village.getType();
    BOOST_CHECK(type == shared::ElementEnum::barbarianVillage);
}

BOOST_AUTO_TEST_SUITE_END()

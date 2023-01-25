#include <shared.hpp>
#include <ctime>
#include <iostream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( BarbarianTest )

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( constructorAndGetters )
{
    shared::Barbarian barbarian;

    shared::ElementEnum type = barbarian.getType();
    BOOST_CHECK(type == shared::ElementEnum::barbarian);
    BOOST_CHECK(barbarian.isAlive == true);
}

BOOST_AUTO_TEST_CASE( kill )
{
    shared::Barbarian barbarian;
    barbarian.kill();
    BOOST_CHECK(barbarian.isAlive == false);
}

BOOST_AUTO_TEST_SUITE_END()

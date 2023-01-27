#include <shared.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( ControlPawnTest )

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( constructorAndGetters )
{
    shared::ControlPawn pawn({20, 15}, "not defined");

    BOOST_CHECK(pawn.getPosition()[0] == 20);
    BOOST_CHECK(pawn.getPosition()[1] == 15);
    BOOST_CHECK(pawn.isReinforced() == false);

    shared::ElementEnum type = pawn.getType();
    BOOST_CHECK(type == shared::ElementEnum::controlPawn);
}

BOOST_AUTO_TEST_CASE( setReinforced )
{
    shared::ControlPawn pawn({0, 0}, "not defined");
    pawn.setReinforced();
    BOOST_CHECK(pawn.isReinforced() == true);
}

BOOST_AUTO_TEST_SUITE_END()

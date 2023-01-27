#include <shared.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( CardTest )

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( constructorAndGetters )
{
    shared::Card card(shared::CardsEnum::science,0,0);

    shared::CardsEnum type = card.getType();
    BOOST_CHECK(type == shared::CardsEnum::science);
    BOOST_CHECK(card.getLevel() == 1);
}

BOOST_AUTO_TEST_CASE( boxAndDifficulty )
{
    shared::Card card(shared::CardsEnum::science,0,0);

    BOOST_CHECK(card.getNumberOfBox() == 0);
    card.addBox(2);
    BOOST_CHECK(card.getNumberOfBox() == 2);
    card.deleteBox(1);
    BOOST_CHECK(card.getNumberOfBox() == 1);

    card.setDificulty(3);
    BOOST_CHECK(card.getDificulty() == 3);
}

BOOST_AUTO_TEST_SUITE_END()

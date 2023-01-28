#include <shared.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( HexagonTest )

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( constructorAndGetters )
{
    shared::Hexagon hexagon;

    BOOST_CHECK(hexagon.getFieldLevel() == shared::FieldLevel::Grassland);
}

BOOST_AUTO_TEST_CASE( setFieldType )
{
    shared::Hexagon hexagon;

    hexagon.setFieldType(shared::FieldLevel::WonderMessa);

    BOOST_CHECK(hexagon.getFieldLevel() == shared::FieldLevel::WonderMessa);
}

BOOST_AUTO_TEST_CASE( manageElements )
{
    shared::Hexagon hexagon;

    BOOST_CHECK(hexagon.getElements().size() == 0);

    std::shared_ptr<shared::Caravan> caravan1 = std::make_shared<shared::Caravan>();
    std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> element1 = *caravan1;
    hexagon.addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element1));

    BOOST_CHECK(hexagon.getElements().size() == 1);

    std::shared_ptr<shared::Caravan> caravan2 = std::make_shared<shared::Caravan>();
    std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> element2 = *caravan2;
    hexagon.addElement(std::make_shared<std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City>>(element2));

    BOOST_CHECK(hexagon.getElements().size() == 2);

    hexagon.removeElement(hexagon.getElements()[0]);

    BOOST_CHECK(hexagon.getElements().size() == 1);

    hexagon.clearElement();

    BOOST_CHECK(hexagon.getElements().size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()

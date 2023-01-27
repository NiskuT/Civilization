#include <shared.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( MapTest )

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( all )
{
    shared::Resource resource(shared::ResourceEnum::stone);

    BOOST_CHECK(resource.getType() == shared::ResourceEnum::stone);
    resource.isAlive = true;
    resource.picked();
    BOOST_CHECK(resource.isAlive == false);


}

BOOST_AUTO_TEST_SUITE_END()
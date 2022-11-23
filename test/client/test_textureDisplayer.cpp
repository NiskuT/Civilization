#include <client.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE( TestTextureDisplayer )

BOOST_AUTO_TEST_CASE( constructorTest )
{
    client::TextureDisplayer texture1("../ressources/img/map/ressource-antic.png");
    BOOST_CHECK(texture1.getSize() == 0);

    client::TextureDisplayer texture2("../ressources/img/map/ressource-stone.png");
    BOOST_CHECK(texture2.getSize() == 0);
}

BOOST_AUTO_TEST_CASE( addSprite )
{
    client::TextureDisplayer texture1("../ressources/img/map/ressource-stone.png");
    texture1.addMapSprite();
    BOOST_CHECK(texture1.getSize() == 1);
    texture1.addMapSprite();
    BOOST_CHECK(texture1.getSize() == 2);
    texture1.addMapSprite();
    BOOST_CHECK(texture1.getSize() == 3);
}

BOOST_AUTO_TEST_SUITE_END()

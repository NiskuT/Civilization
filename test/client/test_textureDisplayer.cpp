#include <client.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE( TestTextureDisplayer )

BOOST_AUTO_TEST_CASE( constructorTest )
{
    /*client::TextureDisplayer texture1("../s/img/map/ressource-antic.png");
    BOOST_CHECK(texture1.getSize() == 0);

    client::TextureDisplayer texture2("../s/img/map/ressource-stone.png");
    BOOST_CHECK(texture2.getSize() == 0);*/
}

BOOST_AUTO_TEST_CASE( addSpriteGetSize )
{
    /*client::TextureDisplayer texture1("../s/img/map/ressource-stone.png");
    texture1.addMapSprite();
    BOOST_CHECK(texture1.getSize() == 1);
    texture1.addMapSprite();
    BOOST_CHECK(texture1.getSize() == 2);
    texture1.addMapSprite();
    BOOST_CHECK(texture1.getSize() == 3);

    client::TextureDisplayer texture2("../s/img/map/ressource-stone.png");
    for(int i = 0; i < 10; i++){
        texture2.addMapSprite();
    }
    BOOST_CHECK(texture2.getSize() == 10);*/
}

/*BOOST_AUTO_TEST_CASE( getWidthHeight )
{
    client::TextureDisplayer texture1("../s/img/map/ressource-stone.png");
    texture1.addMapSprite();
    BOOST_CHECK(texture1.getWidth() == 42);
    BOOST_CHECK(texture1.getHeight() == 43);
    client::TextureDisplayer texture2("../s/img/map/ressource-stone.png");
    BOOST_CHECK(texture2.getWidth() == 0);
    BOOST_CHECK(texture2.getHeight() == 0);
}
BOOST_AUTO_TEST_CASE( getSprite )
{
    client::TextureDisplayer texture1("../s/img/map/ressource-stone.png");
    texture1.addMapSprite();
    BOOST_CHECK((*texture1.getSprite(0)).getLocalBounds().width == 43);
    BOOST_CHECK((*texture1.getSprite(0)).getLocalBounds().height == 42);
}*/

BOOST_AUTO_TEST_SUITE_END()

#include <client.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE( TestTextureData )

BOOST_AUTO_TEST_CASE( constructorTest )
{
    client::TextureData texture1(10, 15, 20, 25, "file1.png");
    BOOST_CHECK(texture1.elementCoord.at(1) == 10);
    BOOST_CHECK(texture1.elementCoord.at(0) == 15);
    BOOST_CHECK(texture1.elementOffset.at(1) == 20);
    BOOST_CHECK(texture1.elementOffset.at(0) == 25);
    BOOST_CHECK(texture1.fileName == "file1.png");

    client::TextureData texture2(30, 40, 50, 60, "file2.png");
    BOOST_CHECK(texture2.elementCoord.at(1) == 30);
    BOOST_CHECK(texture2.elementCoord.at(0) == 40);
    BOOST_CHECK(texture2.elementOffset.at(1) == 50);
    BOOST_CHECK(texture2.elementOffset.at(0) == 60);
    BOOST_CHECK(texture2.fileName == "file2.png");
}

BOOST_AUTO_TEST_CASE( loadTexture )
{
    client::TextureData texture1(0, 4, 20, 21, "file1.png");
    texture1.loadTexture();
    BOOST_CHECK(texture1.position.at(0) == 394);
    BOOST_CHECK(texture1.position.at(1) == 20);
}

BOOST_AUTO_TEST_SUITE_END()

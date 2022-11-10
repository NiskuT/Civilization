#include <shared.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <iostream>
//#include <SFML/Graphics.hpp>

/*
BOOST_AUTO_TEST_CASE(TestSFML)
{
  {
    ::sf::Texture texture;
    BOOST_CHECK(texture.getSize() == ::sf::Vector2<unsigned int> {});
  }
}*/

BOOST_AUTO_TEST_SUITE( TestSuite1 )

struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer ) 
        : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::streambuf * old;
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE( helloTest )
{
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard( output.rdbuf( ) );
        shared::sayHello sayHello("-h");
    }
    BOOST_CHECK(output.is_equal("List of available command:\n-h or -help : display this help\n-hello : display hello world\n"));
    boost::test_tools::output_test_stream output2;
    {
        cout_redirect guard( output2.rdbuf( ) );
        shared::sayHello sayHello("-help");
    }
    BOOST_CHECK(output2.is_equal("List of available command:\n-h or -help : display this help\n-hello : display hello world\n"));
    boost::test_tools::output_test_stream output3;
    {
        cout_redirect guard( output3.rdbuf( ) );
        shared::sayHello sayHello("-hello");
    }
    BOOST_CHECK(output3.is_equal("Hello world !\n" ));
    boost::test_tools::output_test_stream output4;
    {
        cout_redirect guard( output4.rdbuf( ) );
        shared::sayHello sayHello("test");
    }
    BOOST_CHECK(output4.is_equal("Unkown command, use -h for help.\n" ));
}

BOOST_AUTO_TEST_SUITE_END()

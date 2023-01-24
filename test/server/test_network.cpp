#include <shared.hpp>
#include <client.hpp>
#include <server.hpp>
#include <boost/test/unit_test.hpp>
#include <thread>
#include <iostream>

BOOST_AUTO_TEST_SUITE(NetworkTest)

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}
struct ServerFixture
{
    server::Server server;

    ServerFixture()
    {
        std::thread serv([&](){server.start();});
        serv.detach();
        usleep(200000);
    }

    ~ServerFixture()
    {
        server.running.store(false);
    }
};


BOOST_FIXTURE_TEST_CASE(connectionTest, ServerFixture)
{
    client::ClientGameEngine client, client2;
    client.gameId = "new";
    client.connect("127.0.0.1", 8080);

    client.waitToBeReady();
    client.generateMap(10, 10,273884);
    client.loadMap();
    client.handleQuitMenu(true);

    while(server.games.size() == 0){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    client2.gameId = server.games[0]->getId();
    client2.myself->setUsername("testPlayer2");
    client2.connect("127.0.0.1", 8080);
    client2.waitToBeReady();
    client2.loadMap();
    client2.handleQuitMenu(true);
}


BOOST_AUTO_TEST_SUITE_END()
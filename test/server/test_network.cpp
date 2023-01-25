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

BOOST_AUTO_TEST_CASE(connectionTest)
{
    server::Server server;
    std::thread serv([&]()
                     { server.start(8080); });
    serv.detach();
    usleep(200000);

    client::ClientGameEngine client, client2;
    client.gameId = "new";
    client.connect("127.0.0.1", 8080);

    client.waitToBeReady();
    client.generateMap(10, 10, 273884);
    client.loadMap();
    client.handleQuitMenu(true);

    client2.gameId = server.games[0]->getId();
    client2.myself->setUsername("testPlayer2");
    client2.connect("127.0.0.1", 8080);
    client2.waitToBeReady();
    client2.loadMap();
    client2.handleQuitMenu(true);
}

/*BOOST_AUTO_TEST_CASE(stressTest)
{
    server::Server server;
    std::thread serv([&]()
                     { server.start(8081); });
    serv.detach();
    usleep(200000);

    std::array<client::ClientGameEngine, 201> clients;
    
    for (int i = 0; i < 200; i += 3)
    {
        clients[i].myself->setUsername("testPlayer1");
        clients[i].gameId = "new";
        clients[i].connect("127.0.0.1", 8081);

        clients[i].waitToBeReady();
        clients[i].generateMap(10, 10, 273884);
        clients[i].loadMap();

        std::string gameId = clients[i].serverGameId;

        clients[i + 1].gameId = gameId;
        clients[i + 1].myself->setUsername("testPlayer2");
        clients[i + 1].connect("127.0.0.1", 8081);
        clients[i + 1].waitToBeReady();
        clients[i + 1].loadMap();

        clients[i + 2].gameId = gameId;
        clients[i + 2].myself->setUsername("testPlayer3");
        clients[i + 2].connect("127.0.0.1", 8081);
        clients[i + 2].waitToBeReady();
        clients[i + 2].loadMap();
    }
    
    for (int i = 0; i < 202; ++i)
    {
        clients[i].handleQuitMenu(true);
    }
    server.running.store(false);
    sleep(1);
}*/

BOOST_AUTO_TEST_SUITE_END()
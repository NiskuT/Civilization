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

/*BOOST_AUTO_TEST_CASE(connectionTest)
{
    client::ClientGameEngine client, client2;
    server::Server server;

    std::thread serv(&server::Server::start, &server);
    serv.detach();
    usleep(10000);
    client.gameId = "new";
    client.connect("127.0.0.1", 8080);

    client2.generateMap(50, 50,273884);
    client2.loadMap();

    server.running.store(false);
}*/


BOOST_AUTO_TEST_CASE(connectionTest)
{
    client::ClientGameEngine client, client2;
    server::Server server;

    std::thread serv(&server::Server::start, &server);
    serv.detach();
    usleep(10000);
    client.gameId = "new";
    client.connect("127.0.0.1", 8080);

    usleep(10000);
    BOOST_CHECK(server.games.size() == 1);
    BOOST_CHECK(server.games[0]->getPlayers().size() == 1);

    client2.gameId = server.games[0]->getId();
    client2.myself->setUsername("testPlayer2");
    client2.connect("127.0.0.1", 8080);

    usleep(10000);
    BOOST_CHECK(server.games.size() == 1);
    BOOST_CHECK(server.games[0]->getPlayers().size() == 2);

    {
        std::unique_lock<std::mutex> responseLock(client.myself->qAndA.sharedDataMutex);
        client.myself->qAndA.question = "getstate\n";
    }
    client.askServer();
    BOOST_CHECK(client.myself->qAndA.answer.size() > 0);

    {
        std::unique_lock<std::mutex> responseLock(client.myself->qAndA.sharedDataMutex);
        client.myself->qAndA.question = "setmapparam width 18\n";
    }
    client.askServer();
    BOOST_CHECK(client.myself->qAndA.answer.find("ok") != std::string::npos);
    BOOST_CHECK(server.games[0]->gameMap->getMapWidth() == 18);

    client2.myself->qAndA.question = "setmapparam height 24\n";
    client2.askServer();
    BOOST_CHECK(client2.myself->qAndA.answer.find("ok") != std::string::npos);
    BOOST_CHECK(server.games[0]->gameMap->getMapHeight() == 24);

    client2.generateMap(15, 15,273884);
    client.loadMap();

    server.running.store(false);
}

BOOST_AUTO_TEST_SUITE_END()
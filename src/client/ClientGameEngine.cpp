#include <client.hpp>
#include <iostream>

namespace client
{

    ClientGameEngine::ClientGameEngine()
    {
        myself = std::make_shared<shared::Player>();
        myself->setUsername("PlayerTest");
    }

    void ClientGameEngine::connect(const std::string &serverAddress, int serverPort)
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(serverAddress), serverPort);

        boost::asio::ip::tcp::socket clientSocket(io_context);
        try
        {
            clientSocket.connect(endpoint);
        }
        catch (const boost::system::system_error &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            exit(1);
        }
        myself->setSocket(clientSocket);
        myself->state = shared::PlayerState::Connected;

        std::thread t(&ClientGameEngine::startReceiving, this);

        {
            std::lock_guard<std::mutex> lock(myself->socketWriteMutex);
            std::string msg = this->gameId + " " + myself->getName() + "\n";
            boost::asio::write(myself->getSocket(), boost::asio::buffer(msg));
        }

        t.detach();
    }

    void ClientGameEngine::startReceiving()
    {
        std::cout << "Starting receiving" << std::endl;

        while (myself->state == shared::PlayerState::Connected)
        {

            boost::asio::streambuf receiveBuffer;
            boost::system::error_code error;
            {
                std::lock_guard<std::mutex> lock(myself->socketReadMutex);
                boost::asio::read_until(myself->getSocket(), receiveBuffer, '\n', error);
            }

            if (error == boost::asio::error::operation_aborted)
            {
                myself->disconnectPlayer();
                continue;
            }
            if (receiveBuffer.size() > 0)
            {
                std::string messageReceived(
                    boost::asio::buffers_begin(receiveBuffer.data()),
                    boost::asio::buffers_end(receiveBuffer.data()));

                receiveBuffer.consume(receiveBuffer.size());

                if (messageReceived.find("response") == 0)
                {
                    registerServerAnswer(messageReceived);
                }
                else
                {
                    processServerRequest(messageReceived);
                }
            }
        }
    }

    void ClientGameEngine::registerServerAnswer(const std::string &response)
    {
        std::lock_guard<std::mutex> lock(myself->qAndA.sharedDataMutex);
        myself->qAndA.answer = response;
        myself->qAndA.answerReady = true;
        myself->qAndA.condition.notify_one();
    }

    void ClientGameEngine::processServerRequest(const std::string &request)
    {
        std::cout << "Received request: " << request << std::endl;
    }

    void ClientGameEngine::askServer()
    {
        myself->qAndA.answerReady = false;
        {
            std::lock_guard<std::mutex> lock(myself->socketWriteMutex);
            boost::asio::write(myself->getSocket(), boost::asio::buffer(myself->qAndA.question));
        }

        std::unique_lock<std::mutex> responseLock(myself->qAndA.sharedDataMutex);
        myself->qAndA.condition.wait(responseLock, [&]
                                     { return myself->qAndA.answerReady; });

        std::string response = myself->qAndA.answer;
        std::cout << "Received response: " << response << std::endl;
        myself->qAndA.answerReady = false;
    }
}

#include <client.hpp>
#include <iostream>

namespace client
{

    ClientGameEngine::ClientGameEngine(const std::string &serverAddress, int serverPort)
    {
        this->serverAddress = serverAddress;
        this->serverPort = serverPort;
        this->connect();
    }

    void ClientGameEngine::connect()
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
        myself = std::make_shared<shared::Player>(clientSocket);
        myself->state = shared::PlayerState::Connected;

        std::thread t(&ClientGameEngine::startReceiving, this);
        t.join();
    }

    void ClientGameEngine::startReceiving()
    {
        std::cout << "Starting receiving" << std::endl;

        std::unique_lock<std::mutex> lock(myself->socketWriteMutex);
        boost::asio::write(myself->getSocket(), boost::asio::buffer("new quentin\n"));
        lock.unlock();
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
                std::cout << "try received: " << receiveBuffer.size() << std::endl;
                receiveBuffer.consume(receiveBuffer.size());
                std::cout << "Received: " << messageReceived << std::endl;
                // std::cout << "end Received: " << std::endl;

                /*if (messageReceived.find("response") == 0)
                {
                    registerClientAnswer(messageReceived, player);
                }
                else
                {
                    game->processClientRequest(messageReceived, player);
                } */
            }
        }
    }
}

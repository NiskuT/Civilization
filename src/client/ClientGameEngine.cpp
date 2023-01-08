#include <client.hpp>

namespace client
{

    ClientGameEngine::ClientGameEngine(std::string serverAddress, int serverPort)
    {
        this->serverAddress = serverAddress;
        this->serverPort = serverPort;

        this->connect();
    }

    void ClientGameEngine::connect()
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(serverAddress), serverPort);
        try
        {
            clientSocket.connect(endpoint);
        }
        catch (const boost::system::system_error &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            exit(1);
        }

        std::thread t(&ClientGameEngine::startReceiving, this);
        t.detach();
    }

    void ClientGameEngine::disconnect()
    {
        clientSocket.close();
    }

    void ClientGameEngine::startReceiving()
    {

        while (true)
        {

            boost::asio::streambuf receiveBuffer;
            boost::asio::read_until(socket, receiveBuffer, "\n");

            std::string data = boost::asio::buffer_cast<const char *>(receiveBuffer.data());

            processServerData(data);
        }
    }

}

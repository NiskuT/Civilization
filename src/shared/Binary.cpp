#include <shared.hpp>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Serialization.cpp"

using namespace shared;

void Binary::send(std::shared_ptr<shared::Player> player, std::string serializedData)
{
    std::string header = "binary:" + std::to_string(serializedData.size()) + "\n";

    std::lock_guard<std::mutex> lock(player->socketWriteMutex);
    boost::asio::write(player->getSocket(), boost::asio::buffer(header));
    boost::asio::write(player->getSocket(), boost::asio::buffer(serializedData));
}

std::string Binary::receive(std::shared_ptr<shared::Player> player, size_t size)
{
    boost::system::error_code error;
    boost::asio::streambuf receiveBuffer;
    try
    {
        std::lock_guard<std::mutex> lock(player->socketReadMutex);
        boost::asio::read(player->getSocket(), receiveBuffer, boost::asio::transfer_exactly(size), error);
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    if (error == boost::asio::error::operation_aborted || error == boost::asio::error::eof)
    {
        player->disconnectPlayer();
    }
    else if (error)
    {
        std::cerr << "Error: " << error.message() << std::endl;
        player->disconnectPlayer();
    }

    std::string messageReceived(
        boost::asio::buffers_begin(receiveBuffer.data()),
        boost::asio::buffers_end(receiveBuffer.data()));

    return messageReceived;
}

template <typename T>
void Binary::castToObject(std::string receivedData, T &data)
{
    std::stringstream receivedStream(receivedData);
    boost::archive::binary_iarchive ia(receivedStream);
    ia >> data;
}

template <typename T>
void Binary::castToBinary(T &data, std::string &serializedData)
{
    std::stringstream stream;
    boost::archive::binary_oarchive oa(stream);
    oa << data;
    serializedData = stream.str();
}
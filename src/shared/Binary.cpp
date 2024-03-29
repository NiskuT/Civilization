#include <shared.hpp>
#include <iostream>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Serialization.cpp"

using namespace shared;

void Binary::send(std::shared_ptr<shared::Player> player, std::string serializedData, bool answerRequired)
{
    std::string header;
    if (answerRequired)
    {
        header = "binary " + std::to_string(serializedData.size()) + "\n";
    }
    else
    {
        header = "rulesturn " + std::to_string(serializedData.size()) + "\n";
    }
    std::lock_guard<std::mutex> lock(player->socketWriteMutex);
    boost::asio::write(player->getSocket(), boost::asio::buffer(header));
    boost::asio::write(player->getSocket(), boost::asio::buffer(serializedData));
}

std::string Binary::receive(std::shared_ptr<shared::Player> player, std::istream &alreadyReceived, size_t totalSize)
{
    std::stringstream buffer;
    buffer << alreadyReceived.rdbuf();
    std::string fullMessage = buffer.str();

    boost::system::error_code error;
    boost::asio::streambuf receiveBuffer;

    if (fullMessage.size() > totalSize)
    {
        for (size_t i = fullMessage.size(); i > totalSize; i--)
        {
            alreadyReceived.unget();
        }

        return fullMessage.substr(0, totalSize);
    }
    else if (fullMessage.size() == totalSize)
    {
        return fullMessage;
    }
    size_t size = totalSize - fullMessage.size();

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

    if (fullMessage.size() == 0)
    {
        return messageReceived;
    }
    else
    {
        fullMessage += messageReceived;
        return fullMessage;
    }
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

// We need to instantiate the templates for the types we use in other library than shared
template void Binary::castToBinary<Map>(Map &data, std::string &serializedData);
template void Binary::castToObject<Map>(std::string receivedData, Map &data);

template void Binary::castToBinary<RuleArgsStruct>(RuleArgsStruct &data, std::string &serializedData);
template void Binary::castToObject<RuleArgsStruct>(std::string receivedData, RuleArgsStruct &data);

template void Binary::castToBinary<Barbarian>(Barbarian &data, std::string &serializedData);
template void Binary::castToObject<Barbarian>(std::string receivedData, Barbarian &data);

template void Binary::castToBinary<BarbarianVillage>(BarbarianVillage &data, std::string &serializedData);
template void Binary::castToObject<BarbarianVillage>(std::string receivedData, BarbarianVillage &data);

template void Binary::castToBinary<City>(City &data, std::string &serializedData);
template void Binary::castToObject<City>(std::string receivedData, City &data);

template void Binary::castToBinary<Caravan>(Caravan &data, std::string &serializedData);
template void Binary::castToObject<Caravan>(std::string receivedData, Caravan &data);

template void Binary::castToBinary<ControlPawn>(ControlPawn &data, std::string &serializedData);
template void Binary::castToObject<ControlPawn>(std::string receivedData, ControlPawn &data);

template void Binary::castToBinary<Hexagon>(Hexagon &data, std::string &serializedData);
template void Binary::castToObject<Hexagon>(std::string receivedData, Hexagon &data);

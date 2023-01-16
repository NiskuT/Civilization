#include <client.hpp>

#define TCHAT_MAX_SIZE 300

using namespace client;

/*!
 * @brief Load all the chat
 */
Chat::Chat(sf::Font& font)
{
    for(unsigned i = 0; i < gameChat.size(); i++ )
    {
        gameChat[i].setString("");
        gameChat[i].setFont(font);
        gameChat[i].setCharacterSize(15);
        gameChat[i].setFillColor(sf::Color::Black);
        gameChat[i].setPosition(20, 305 + 20 * i);
    }
    chatButton.emplace_back(sf::Vector2f(340, 265), sf::Vector2f(0, 300), sf::Color(255, 255, 255, 180), false);
    chatButton.emplace_back(sf::Vector2f(300, 30), sf::Vector2f(20, 515), sf::Color(255, 255, 255, 180), false);
    chatButton.back().setText(15, sf::Vector2f(0, 0), "", font, 100);

    updateChat("19:45", "Lasso", "Game is starting");
    updateChat("19:50", "Lasso", "Game");
    updateChat("19:51", "Lasso", "How are you ?");
    updateChat("19:53", "Lasso", "Game is starting");
}

/*!
 * @brief Move the index of text to i --
 */
void Chat::incrementChat()
{
    for(unsigned i = 1; i < gameChat.size(); i++ )
    {
        gameChat[i-1].setString(gameChat[i].getString());
    }
}

/*!
 * @brief update the Chat with a string
 * @param time when the message is sent
 * @param username user of the player that send the message
 * @param sendMessage message to be add to the chat
 */
void Chat::updateChat(std::string time, std::string username, std::string sendMessage)
{
    std::unique_lock<std::mutex> lock(mutexChat);
    message.clear();
    lock.unlock();
    chatButton[1].buttonText->setString("");

    if (!username.empty())
    {
        sendMessage = time + " [" + username + "]: " + sendMessage;
    }

    incrementChat();
    std::string secondLine = "";

    gameChat[9].setString(sendMessage);

    while (gameChat[9].getGlobalBounds().width > TCHAT_MAX_SIZE)
    {
        std::string nextChar = gameChat[9].getString();
        nextChar = nextChar.back();
        secondLine.insert(0, nextChar);
        sendMessage.pop_back();
        gameChat[9].setString(sendMessage);
    }

    if (!secondLine.empty())
    {
        updateChat("", "", secondLine);
    }
}

/*!
 * @brief Add a char to the chat message
 */
void Chat::addChatChar(std::string ch)
{
    std::unique_lock<std::mutex> lock(mutexChat);
    message += ch;
    lock.unlock();
    chatButton[1].buttonText->setString(chatButton[1].buttonText->getString() + ch);

    while(chatButton[1].buttonText->getGlobalBounds().width > TCHAT_MAX_SIZE)
    {
        std::string newString = chatButton[1].buttonText->getString();
        newString.erase(0, 1);
        chatButton[1].buttonText->setString(newString);
    }
    chatButton[1].centerText(true);
}

/*!
 * @brief Delete a char to the chat message
 */
void Chat::deleteChatChar()
{
    std::string newString = chatButton[1].buttonText->getString();
    if (!newString.empty())
    {
        newString.pop_back();
        std::lock_guard<std::mutex> lock(mutexChat);
        message.pop_back();
    }
    chatButton[1].buttonText->setString(newString);
    chatButton[1].centerText(false);
}

/*!
 * @brief display the entire Chat
 */
void Chat::drawChat(std::shared_ptr<sf::RenderWindow> window)
{
    for (unsigned i = 0; i < chatButton.size(); i++)
    {
        window->draw(*chatButton[i].buttonRect);
    }
    window->draw(*chatButton[1].buttonText);
    for (auto &chat : gameChat)
    {
        window->draw(chat);
    }
}
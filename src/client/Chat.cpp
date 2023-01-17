#include <client.hpp>

#define CHAT_MAX_SIZE 300

#define NUMBER_LINE 9

#define CHAT_START_POSITION 5
#define CHAT_SIZE_X 300
#define CHAT_SIZE_Y 265
#define CHAT_OFFSET 20
#define CHAT_TEXT_SIZE 30
#define CHAT_FONT_SIZE 15

#define BUTTON_TEXT_ENTER 1

const sf::Color TEXT_COLOR = sf::Color(255, 255, 255, 180);

using namespace client;

/*!
 * @brief Load all the chat
 */
Chat::Chat(sf::Font& font)
{
    unsigned i;
    for(i = 0; i < gameChat.size(); i++ )
    {
        gameChat[i].setString("");
        gameChat[i].setFont(font);
        gameChat[i].setCharacterSize(CHAT_FONT_SIZE);
        gameChat[i].setFillColor(sf::Color::Black);
        gameChat[i].setPosition(CHAT_OFFSET, CHAT_START_POSITION + CHAT_SIZE_X + CHAT_OFFSET * i);
    }
    chatButton.emplace_back(
        sf::Vector2f(CHAT_SIZE_X + 2 * CHAT_SIZE_X, CHAT_SIZE_Y), 
        sf::Vector2f(0, CHAT_SIZE_X), 
        TEXT_COLOR, 
        false);

    chatButton.emplace_back(
        sf::Vector2f(CHAT_SIZE_X, CHAT_TEXT_SIZE), 
        sf::Vector2f(CHAT_SIZE_X, CHAT_START_POSITION + CHAT_SIZE_X + CHAT_OFFSET * (i + 1)), 
        TEXT_COLOR, 
        false);

    chatButton.back().setText(CHAT_FONT_SIZE, sf::Vector2f(0, 0), "", font, CHAT_SIZE_X);

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
    chatButton[BUTTON_TEXT_ENTER].buttonText->setString("");

    if (!username.empty())
    {
        sendMessage = time + " [" + username + "]: " + sendMessage;
    }

    incrementChat();
    std::string secondLine = "";

    gameChat[NUMBER_LINE].setString(sendMessage);

    while (gameChat[NUMBER_LINE].getGlobalBounds().width > CHAT_MAX_SIZE)
    {
        std::string nextChar = gameChat[NUMBER_LINE].getString();
        nextChar = nextChar.back();
        secondLine.insert(0, nextChar);
        sendMessage.pop_back();
        gameChat[NUMBER_LINE].setString(sendMessage);
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
    chatButton[BUTTON_TEXT_ENTER].buttonText->setString(chatButton[BUTTON_TEXT_ENTER].buttonText->getString() + ch);

    while(chatButton[BUTTON_TEXT_ENTER].buttonText->getGlobalBounds().width > CHAT_MAX_SIZE)
    {
        std::string newString = chatButton[BUTTON_TEXT_ENTER].buttonText->getString();
        newString.erase(0, 1);
        chatButton[BUTTON_TEXT_ENTER].buttonText->setString(newString);
    }
    chatButton[BUTTON_TEXT_ENTER].centerText(true);
}

/*!
 * @brief Delete a char to the chat message
 */
void Chat::deleteChatChar()
{
    std::string newString = chatButton[BUTTON_TEXT_ENTER].buttonText->getString();
    if (!newString.empty())
    {
        newString.pop_back();
        std::lock_guard<std::mutex> lock(mutexChat);
        message.pop_back();
    }
    chatButton[BUTTON_TEXT_ENTER].buttonText->setString(newString);
    chatButton[BUTTON_TEXT_ENTER].centerText(false);
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
    window->draw(*chatButton[BUTTON_TEXT_ENTER].buttonText);
    for (auto &chat : gameChat)
    {
        window->draw(chat);
    }
}
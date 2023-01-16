#include <client.hpp>

#define TCHAT_MAX_SIZE 300

using namespace client;

/*!
 * @brief Load all the tchat
 */
Tchat::Tchat(sf::Font& font)
{
    for(unsigned i = 0; i < gameTchat.size(); i++ )
    {
        gameTchat[i].setString("");
        gameTchat[i].setFont(font);
        gameTchat[i].setCharacterSize(15);
        gameTchat[i].setFillColor(sf::Color::Black);
        gameTchat[i].setPosition(20, 305 + 20 * i);
    }
    tchatButton.emplace_back(sf::Vector2f(340, 265), sf::Vector2f(0, 300), sf::Color(255, 255, 255, 180), false);
    tchatButton.emplace_back(sf::Vector2f(300, 30), sf::Vector2f(20, 515), sf::Color(255, 255, 255, 180), false);
    tchatButton.back().setText(15, sf::Vector2f(0, 0), "", font, 100);

    updateTchat("19:45", "Lasso", "Game is starting");
    updateTchat("19:50", "Lasso", "Game");
    updateTchat("19:51", "Lasso", "How are you ?");
    updateTchat("19:53", "Lasso", "Game is starting");
}

/*!
 * @brief Move the index of text to i --
 */
void Tchat::incrementTchat()
{
    for(unsigned i = 1; i < gameTchat.size(); i++ )
    {
        gameTchat[i-1].setString(gameTchat[i].getString());
    }
}

/*!
 * @brief update the Tchat with a string
 * @param time when the message is sent
 * @param username user of the player that send the message
 * @param sendMessage message to be add to the tchat
 */
void Tchat::updateTchat(std::string time, std::string username, std::string sendMessage)
{
    message.clear();
    tchatButton[1].buttonText->setString("");

    if (!username.empty())
    {
        sendMessage = time + " [" + username + "]: " + sendMessage;
    }

    incrementTchat();
    std::string secondLine = "";

    gameTchat[9].setString(sendMessage);

    while (gameTchat[9].getGlobalBounds().width > TCHAT_MAX_SIZE)
    {
        std::string nextChar = gameTchat[9].getString();
        nextChar = nextChar.back();
        secondLine.insert(0, nextChar);
        sendMessage.pop_back();
        gameTchat[9].setString(sendMessage);
    }

    if (!secondLine.empty())
    {
        updateTchat("", "", secondLine);
    }
}

/*!
 * @brief Add a char to the tchat message
 */
void Tchat::addTchatChar(std::string ch)
{
    message += ch;
    tchatButton[1].buttonText->setString(tchatButton[1].buttonText->getString() + ch);

    while(tchatButton[1].buttonText->getGlobalBounds().width > TCHAT_MAX_SIZE)
    {
        std::string newString = tchatButton[1].buttonText->getString();
        newString.erase(0, 1);
        tchatButton[1].buttonText->setString(newString);
    }
    tchatButton[1].centerText(true);
}

/*!
 * @brief Delete a char to the tchat message
 */
void Tchat::deleteTchatChar()
{
    std::string newString = tchatButton[1].buttonText->getString();
    if (!newString.empty())
    {
        newString.pop_back();
        message.pop_back();
    }
    tchatButton[1].buttonText->setString(newString);
    tchatButton[1].centerText(false);
}

/*!
 * @brief display the entire Tchat
 */
void Tchat::drawTchat(std::shared_ptr<sf::RenderWindow> window)
{
    for (unsigned i = 0; i < tchatButton.size(); i++)
    {
        window->draw(*tchatButton[i].buttonRect);
    }
    window->draw(*tchatButton[1].buttonText);
    for (auto &tchat : gameTchat)
    {
        window->draw(tchat);
    }
}
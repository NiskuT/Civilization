#include <client.hpp>
#include <iostream>


namespace client
{

/*!
 * \brief Constructor
 *
 * Constructor of Button class
 *
 * @param buttonSize size of the Button
 * @param buttonPos position of the Button
 * @param buttonColor color of the Button
 * @param border true if the border is red, false if the border is black
 */
Button::Button(sf::Vector2f buttonSize, sf::Vector2f buttonPos, sf::Color buttonColor, bool border)
{
    buttonRect = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape);
    buttonRect->setSize(buttonSize);
    buttonRect->setPosition(buttonPos);
    buttonRect->setFillColor(buttonColor);
    if (border) 
    {
        redBorder = true;
        buttonRect->setOutlineColor(sf::Color::Red);
        buttonRect->setOutlineThickness(2.0f);
    }
    else 
    {
        redBorder = false;
        buttonRect->setOutlineColor(sf::Color::Black);
        buttonRect->setOutlineThickness(1.0f);
    }
}

/*!
 * \brief Set the text of the Button
 *
 * @param textSize size of the text
 * @param textOffset offset of the text
 * @param text text of the Button
 * @param font font of the text
 */
void Button::setText(int textSize, sf::Vector2f textOffset, std::string text, sf::Font &font, int size)
{
    maxTextSize = size;
    buttonText = std::unique_ptr<sf::Text>(new sf::Text);
    buttonText->setFont(font);
    buttonText->setString(text);
    buttonText->setCharacterSize(textSize);
    centerText(true, textOffset);
    buttonText->setFillColor(sf::Color::Black);
}

void Button::addChar(std::string ch)
{
    buttonText->setString(buttonText->getString() + ch);
    if (buttonText->getLocalBounds().width >= buttonRect->getLocalBounds().width)
    {
        std::cout << "Too long\n";
        buttonText->setCharacterSize(buttonText->getCharacterSize() - 1);
    }
    centerText(false);
}

void Button::centerText(bool centerAllAxis, sf::Vector2f textOffset)
{
    sf::Vector2f buttonPos = buttonRect->getPosition();
    sf::Vector2f buttonSize = buttonRect->getSize();
    int yPosText;
    if (centerAllAxis)
    {
        yPosText = buttonPos.y + (buttonSize.y - buttonText->getGlobalBounds().height) / 2  - buttonText->getGlobalBounds().height/2 + textOffset.y ;
    }
    else
    {
        yPosText = buttonText->getPosition().y;
    }
    int xPosText = buttonPos.x + (buttonSize.x - buttonText->getGlobalBounds().width) / 2 + textOffset.x;
    buttonText->setPosition(sf::Vector2f(xPosText, yPosText));
}


}
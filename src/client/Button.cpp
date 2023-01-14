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
    buttonRect = std::shared_ptr<sf::RectangleShape>(new sf::RectangleShape);
    buttonRect->setSize(buttonSize);
    buttonRect->setPosition(buttonPos);
    buttonRect->setFillColor(buttonColor);
    if (border) 
    {
        setActive();
    }
    else 
    {
        setInactive();
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
    buttonText = std::shared_ptr<sf::Text>(new sf::Text);
    buttonText->setFont(font);
    buttonText->setString(text);
    buttonText->setCharacterSize(textSize);
    centerText(true, textOffset);
    buttonText->setFillColor(sf::Color::Black);
}

/*!
 * \brief Add a char to the text
 *
 * @param ch char to be add
 */
void Button::addChar(std::string ch)
{
    buttonText->setString(buttonText->getString() + ch);
    if (buttonText->getLocalBounds().width >= buttonRect->getLocalBounds().width)
    {
        delChar();
    }
    centerText(false);
}

/*!
 * \brief Delete a char to the text
 */
void Button::delChar()
{
    std::string newString = buttonText->getString();
    newString.pop_back();
    buttonText->setString(newString);
    centerText(false);
}

/*!
 * \brief Center the text
 *
 * @param centerAllAxis true if you also want to center on y Axis
 * @param textOffset offset on x
 */
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

/*!
 * \brief Put the border into red border
 */
void Button::setActive()
{
    redBorder = true;
    buttonRect->setOutlineColor(sf::Color::Red);
    buttonRect->setOutlineThickness(2.0f);
}

/*!
 * \brief Put the border into black border
 */
void Button::setInactive()
{
    redBorder = false;
    buttonRect->setOutlineColor(sf::Color::Black);
    buttonRect->setOutlineThickness(1.0f);
}

/*!
 * \brief Click on the button
 */
bool Button::clickButton()
{
    if(maxTextSize != 0)
    {
        setActive();
        return false;
    }
    else{
        return true;
    }
}

}
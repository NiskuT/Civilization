#include <client.hpp>


namespace client
{

Button::Button(sf::Vector2f buttonSize, sf::Vector2f buttonPos, sf::Color buttonColor, bool border)
{
    buttonRect = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape);
    buttonRect->setSize(buttonSize);
    buttonRect->setPosition(buttonPos);
    buttonRect->setFillColor(buttonColor);
    if (border) {
        redBorder = true;
        buttonRect->setOutlineColor(sf::Color::Red);
        buttonRect->setOutlineThickness(2.0f);
    }
    else {
        redBorder = false;
        buttonRect->setOutlineColor(sf::Color::Black);
        buttonRect->setOutlineThickness(1.0f);
    }

    
}

void Button::setText(int textSize, sf::Vector2f textOffset, std::string text, sf::Font *font)
{
    buttonText = std::unique_ptr<sf::Text>(new sf::Text);
    buttonText->setFont(*font);
    buttonText->setString(text);
    buttonText->setCharacterSize(textSize);
    sf::Vector2f buttonPos = buttonRect->getPosition();
    sf::Vector2f buttonSize = buttonRect->getSize();
    int xPosText = buttonPos.x + (buttonSize.x - buttonText->getGlobalBounds().width) / 2 + textOffset.x;
    int yPosText = buttonPos.y + (buttonSize.y - buttonText->getGlobalBounds().height) / 2  - buttonText->getGlobalBounds().height/2 + textOffset.y ;
    buttonText->setPosition(sf::Vector2f(xPosText, yPosText));
    buttonText->setFillColor(sf::Color::Black);
}


}
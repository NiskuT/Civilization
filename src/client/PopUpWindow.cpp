#include <client.hpp>
#include <iostream>
#include <fstream>

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

const sf::Color BACKGROUND_COLOR = sf::Color(0, 0, 0, 150);
const sf::Color NUMBER_BUTTON_COLOR = sf::Color(247, 200, 195, 130);

using namespace client;

void setImage(std::unique_ptr<TextureDisplayer> &texture, std::string path, sf::Vector2f position, sf::Vector2f scale)
{
    texture = std::make_unique<TextureDisplayer>(RESOURCES_PATH + path);
    texture->addSprite();
    texture->getSprite().setPosition(position);
    texture->getSprite().setScale(scale);
}

void setText(std::unique_ptr<sf::Text> &text, sf::Font &font, int size)
{
    text = std::make_unique<sf::Text>("", font, size);
    text->setFillColor(sf::Color::Black);
}

void PopUpWindow::setUpValidateBoxesWindow()
{

    if (!font.loadFromFile(RESOURCES_PATH "/font/Calibri.ttf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }

    setImage(littleBackground,
             "/pop-up/little-background.png",
             sf::Vector2f(data["little-background-pos-x"].asFloat(), data["little-background-pos-y"].asFloat()),
             sf::Vector2f(data["little-background-scale-x"].asFloat(), 1));

    std::string questionString = "You have X boxes, \nHow many boxes do you want to play?";
    title = std::make_unique<sf::Text>(questionString, font, data["text-size"].asInt());
    title->setFillColor(sf::Color::Black);
    title->setPosition(
        windowLength / 2 - title->getGlobalBounds().width / 2,
        littleBackground->getSprite().getPosition().y + data["text-offset-y"].asInt());

    chooseNumberOfBoxesButton = std::make_unique<Button>(
        sf::Vector2f(data["boxes-button-size"].asInt(), data["boxes-button-size"].asInt()),
        sf::Vector2f(data["boxes-button-pos-x"].asInt(), data["boxes-button-pos-y"].asInt()),
        NUMBER_BUTTON_COLOR,
        false);
    chooseNumberOfBoxesButton->setText(data["boxes-button-text-size"].asInt(), sf::Vector2f(0, 0), "X", font);

    setImage(arrowLessTexture,
             "/pop-up/arrow-less.png",
             sf::Vector2f(data["arrow-less-pos-x"].asFloat(), data["arrow-less-pos-y"].asFloat()),
             sf::Vector2f(1, 1));

    setImage(arrowMoreTexture,
             "/pop-up/arrow-more.png",
             sf::Vector2f(data["arrow-more-pos-x"].asFloat(), data["arrow-more-pos-y"].asFloat()),
             sf::Vector2f(1, 1));

    setImage(doneTexture,
             "/pop-up/done.png",
             sf::Vector2f(data["done-pos-x"].asFloat(), data["done-pos-y"].asFloat()),
             sf::Vector2f(1, 1));
}

/*!
 * @brief Constructor
 * Constructor of PopUpWindow class
 * @param windowLength length of the window
 * @param windowWidth width of the window
 */
PopUpWindow::PopUpWindow(int windowLength, int windowWidth, const Json::Value &data, bool isActive)
{
    this->data = data;
    this->windowLength = windowLength;
    this->windowWidth = windowWidth;

    blackBackground = std::make_unique<Button>(sf::Vector2f(windowLength, windowWidth), sf::Vector2f(0, 0), BACKGROUND_COLOR, false);

    if (isActive)
    {
        setUpValidateBoxesWindow();
    }

    else
    {
        if (!font.loadFromFile(RESOURCES_PATH "/font/MorrisRomanBlack.otf"))
        {
            std::cerr << "Font not loaded" << std::endl;
        }

        setImage(littleBackground,
                 "/pop-up/little-background.png",
                 sf::Vector2f(data["winner-little-background-pos-x"].asFloat(), data["winner-little-background-pos-y"].asFloat()),
                 sf::Vector2f(data["winner-scale"].asFloat(), data["winner-scale"].asFloat()));

        setText(title, font, data["winner-text-size"].asInt());

        setText(body, font, data["text-size"].asInt());

        centerText();
    }
}


void drawGeneral(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::RectangleShape> &blackBackgroundButton, sf::Sprite littleBackground, std::unique_ptr<sf::Text> &title)
{
    window->draw(*blackBackgroundButton);
    window->draw(littleBackground);
    window->draw(*title);
}

void PopUpWindow::drawWinnerWindow(std::shared_ptr<sf::RenderWindow> window)
{
    drawGeneral(window, blackBackground->buttonRect, littleBackground->getSprite(), title);
    window->draw(*body);
}

void PopUpWindow::drawValidateBoxesButtons(std::shared_ptr<sf::RenderWindow> window)
{
    drawGeneral(window, blackBackground->buttonRect, littleBackground->getSprite(), title);
    window->draw(*chooseNumberOfBoxesButton->buttonRect);
    window->draw(*chooseNumberOfBoxesButton->buttonText);
    window->draw(arrowLessTexture->getSprite());
    window->draw(arrowMoreTexture->getSprite());
    window->draw(doneTexture->getSprite());
}

void PopUpWindow::centerText()
{
    body->setPosition(
        windowLength / 2 - body->getGlobalBounds().width / 2,
        littleBackground->getSprite().getPosition().y + data["winner-body-offset-y"].asInt());

    title->setPosition(
        windowLength / 2 - title->getGlobalBounds().width / 2,
        littleBackground->getSprite().getPosition().y + data["winner-text-offset-y"].asInt());
}

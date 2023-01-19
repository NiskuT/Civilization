#include <client.hpp>
#include <iostream>
#include <fstream>

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

const sf::Color BACKGROUND_COLOR = sf::Color(0, 0, 0, 150);
const sf::Color NUMBER_BUTTON_COLOR = sf::Color(247, 200, 195, 240);


using namespace client;

void setImage(std::unique_ptr<TextureDisplayer>& texture, std::string path, sf::Vector2f position, sf::Vector2f scale) 
{
    texture = std::make_unique<TextureDisplayer>(RESOURCES_PATH + path);
    texture->addSprite();
    texture->getSprite().setPosition(position);
    texture->getSprite().setScale(scale);
}

ValidateBoxesButtons::ValidateBoxesButtons(int windowLength, int windowWidth)
{
    const Json::Value &data = gameWindow->openJsonFile("/img/validateBoxes/dataButton.json");

    blackBackground = std::make_unique<Button>(sf::Vector2f(windowLength, windowWidth), sf::Vector2f(0, 0), BACKGROUND_COLOR, false);

     if (!font.loadFromFile(RESOURCES_PATH "/img/hud/Calibri.ttf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }
   
    setImage(
        littleBackground,
        "/img/validateBoxes/little-background.png",
        sf::Vector2f(data["little-background-pos-x"].asFloat(), data["little-background-pos-y"].asFloat()),
        sf::Vector2f(data["little-background-scale-x"].asFloat(), 1));

    std::string questionString = "You have X boxes, \nHow many boxes do you want to play?";
    question = std::make_unique<sf::Text>(questionString, font, data["text-size"].asInt());
    question->setFillColor(sf::Color::Black);
    question->setPosition(
        windowLength/2 - question->getGlobalBounds().width/2, 
        littleBackground->getSprite().getPosition().y + data["text-offset-y"].asInt());


    chooseNumberOfBoxesButton = std::make_unique<Button>(
        sf::Vector2f(data["boxes-button-size"].asInt(), data["boxes-button-size"].asInt()), 
        sf::Vector2f(data["boxes-button-pos-x"].asInt(), data["boxes-button-pos-y"].asInt()), 
        NUMBER_BUTTON_COLOR, 
        false);
    chooseNumberOfBoxesButton->setText(data["boxes-button-text-size"].asInt(), sf::Vector2f(0, 0) , "X", font);

    setImage(
        arrowLessTexture, 
        "/img/validateBoxes/arrow-less.png", 
        sf::Vector2f(data["arrow-less-pos-x"].asFloat(), data["arrow-less-pos-y"].asFloat()),
        sf::Vector2f(1, 1));

    setImage(
        arrowMoreTexture, 
        "/img/validateBoxes/arrow-more.png", 
        sf::Vector2f(data["arrow-more-pos-x"].asFloat(), data["arrow-more-pos-y"].asFloat()),
        sf::Vector2f(1, 1));

    setImage(
        doneTexture, 
        "/img/validateBoxes/done.png", 
        sf::Vector2f(data["done-pos-x"].asFloat(), data["done-pos-y"].asFloat()), 
        sf::Vector2f(1, 1));

}


void ValidateBoxesButtons::drawValidateBoxesButtons(std::shared_ptr<sf::RenderWindow> window){
    window->draw(*blackBackground->buttonRect);
    window->draw(littleBackground->getSprite());
    window->draw(*question);
    window->draw(*chooseNumberOfBoxesButton->buttonRect);
    window->draw(*chooseNumberOfBoxesButton->buttonText);
    window->draw(arrowLessTexture->getSprite());
    window->draw(arrowMoreTexture->getSprite());
    window->draw(doneTexture->getSprite());

}

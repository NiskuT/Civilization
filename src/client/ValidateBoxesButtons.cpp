#include <client.hpp>
#include <iostream>
#include <fstream>

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

const sf::Color BACKGROUND_COLOR = sf::Color(0, 0, 0, 150);
const sf::Color LITTLE_BACKGROUND_COLOR = sf::Color(247, 200, 195, 240);
const sf::Color NUMBER_BUTTON_COLOR = sf::Color(150, 90, 90, 240);



using namespace client;

ValidateBoxesButtons::ValidateBoxesButtons(int windowLength, int windowWidth)
{
    /*
    // open JSON File
    std::ifstream file(RESOURCES_PATH "/img/validateBoxes/dataButton.json");

    if (!file.is_open())
    {
        std::cerr << "Error while opening json ressources file" << std::endl;
        std::cerr << "/img/validateBoxes/dataButton.json" << std::endl;
        exit(1);
    }
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::unique_ptr<Json::CharReader> reader = std::unique_ptr<Json::CharReader>(Json::CharReaderBuilder().newCharReader());
    Json::Value obj;
    std::string errors;
    reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

    const Json::Value &data = obj["data"];  
    */  


    blackBackground = std::make_unique<Button>(sf::Vector2f(windowLength, windowWidth), sf::Vector2f(0, 0), BACKGROUND_COLOR, false);

     if (!font.loadFromFile(RESOURCES_PATH "/img/hud/Calibri.ttf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }
    sf::Vector2f littleBackgroundSize = sf::Vector2f(480, 220);
    sf::Vector2f littleBackgroundPos = sf::Vector2f(windowLength/2 - littleBackgroundSize.x/2, windowWidth/2 - littleBackgroundSize.y/2);
    littleBackground = std::make_unique<Button>(littleBackgroundSize, littleBackgroundPos, LITTLE_BACKGROUND_COLOR, false);
    littleBackground->setText(27, sf::Vector2f(0, -littleBackgroundSize.y/2 + 70) , "You have X boxes, \nHow many boxes do you want to play?", font);

    chooseNumberOfBoxesButton = std::make_unique<Button>(sf::Vector2f(50, 50), sf::Vector2f(775, 450), NUMBER_BUTTON_COLOR, false);
    chooseNumberOfBoxesButton->setText(30, sf::Vector2f(0, 0) , "X", font);


    arrowLessTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/img/validateBoxes/arrow-less.png");
    arrowLessTexture->addSprite();
    arrowLessTexture->getSprite().setPosition(822, 470);

    arrowMoreTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/img/validateBoxes/arrow-more.png");
    arrowMoreTexture->addSprite();
    arrowMoreTexture->getSprite().setPosition(822, 440);

    doneTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/img/validateBoxes/done.png");
    doneTexture->addSprite();
    doneTexture->getSprite().setPosition(littleBackgroundPos.x + littleBackgroundSize.x - doneTexture->getWidth(), littleBackgroundPos.y + littleBackgroundSize.y - doneTexture->getHeight());

}

void ValidateBoxesButtons::drawValidateBoxesButtons(std::shared_ptr<sf::RenderWindow> window){
    window->draw(*blackBackground->buttonRect);
    window->draw(*littleBackground->buttonRect);
    window->draw(*littleBackground->buttonText);
    window->draw(*chooseNumberOfBoxesButton->buttonRect);
    window->draw(*chooseNumberOfBoxesButton->buttonText);
    window->draw(arrowLessTexture->getSprite());
    window->draw(arrowMoreTexture->getSprite());
    window->draw(doneTexture->getSprite());

}

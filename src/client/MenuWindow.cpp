#include <client.hpp>
#include <iostream>
#include <json/json.h>
#include <fstream>
#include <string>
#include <codecvt>

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define REFRESH_TIME 30

#define BUTTON_CREAT 1
#define BUTTON_USERNAME 2
#define BUTTON_SERVER 3
#define BUTTON_PORT 4
#define BUTTON_LOAD 4
#define BUTTON_ID 5
#define BUTTON_PLAYER 5
#define BUTTON_SEED 6
#define BUTTON_CONNECT 6
#define BUTTON_START 7

#define ASCI_BEGIN 41
#define ASCI_END 123

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

const sf::Color buttonColor = sf::Color(247, 200, 195, 255);

namespace client
{

/*!
 * \brief Constructor
 *
 * Constructor of MenuWindow class
 */
MenuWindow::MenuWindow()
{
    loadMenuTexture();
    currentMenu = &menuButtons;
    currentText = &menuTexts;
}

/*!
 * \brief Display all the menu on the screen
 */
void MenuWindow::displayWindow()
{

    clientMenuWindow->clear(sf::Color::Blue);

    clientMenuWindow->draw(backgroundTexture->getSprite());

    for (unsigned i = 0; i < currentMenu->size(); i++)
    {
        clientMenuWindow->draw(*currentMenu->at(i).buttonRect);
        clientMenuWindow->draw(*currentMenu->at(i).buttonText);
    }

    for (unsigned i = 0; i < currentText->size(); i++)
    {
        clientMenuWindow->draw(currentText->at(i));
    }
    
    clientMenuWindow->display();
}

/*!
 * \brief Loop that look for events to happend and call displayWindow()
 * @param clientWindow is window that comes from the engine
 * @param quitGame is the function used to quit the menu, it is load as an attribut
 */
void MenuWindow::startMenu(std::shared_ptr<sf::RenderWindow> clientWindow, std::function<void(bool)> quitGame, std::function<void(std::string, std::string)> connectGame, std::function<void(std::string, std::string, int)> createGame)
{
    quitMenuWindow = quitGame;
    clientMenuWindow = clientWindow;
    tryConnectGame = connectGame;
    creatNewGame = createGame;

    long lastUpdateTimer = getCurrentTime(false);

    while (clientMenuWindow->isOpen())
    {

        if (getCurrentTime(false) - lastUpdateTimer > (REFRESH_TIME))
        {
            displayWindow();
            lastUpdateTimer = getCurrentTime(false);
        }

        // handle events
        sf::Event event;
        while (clientMenuWindow->pollEvent(event))
        {
            if (menuEventHappened(event))
            {
                return;
            }
        }
    }
}

/*!
 * \brief Test events and do actions corresponding to the event
 * @param event pointer to the event
 */
bool MenuWindow::menuEventHappened(sf::Event& event){

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::string utf8;
    sf::Vector2i clickPoint;
    bool clickAction;

    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:

        clickPoint = sf::Mouse::getPosition(*clientMenuWindow);

        for (unsigned i = 0; i < currentMenu->size(); i++)
        {
            clickAction = false;
            currentMenu->at(i).setInactive();
            if (  clickPoint.x >= currentMenu->at(i).buttonRect->getGlobalBounds().left 
                && clickPoint.x <= currentMenu->at(i).buttonRect->getGlobalBounds().left + currentMenu->at(i).buttonRect->getGlobalBounds().width 
                && clickPoint.y >= currentMenu->at(i).buttonRect->getGlobalBounds().top 
                && clickPoint.y <= currentMenu->at(i).buttonRect->getGlobalBounds().top + currentMenu->at(i).buttonRect->getGlobalBounds().height)
            {
                clickAction = currentMenu->at(i).clickButton();
            }
            if (clickAction && i==BUTTON_CREAT && currentMenu == &menuButtons)
            {
                currentMenu = &newGameButtons;
                currentText = &newGameTexts;
            }
            else if (clickAction && i==BUTTON_CONNECT && currentMenu == &menuButtons)
            {
                tryConnectGame(menuButtons[BUTTON_ID].buttonText->getString(), menuButtons[BUTTON_USERNAME].buttonText->getString());
                quitMenuWindow(false);
                return true;
            }
            else if (clickAction && i==BUTTON_LOAD && currentMenu == &newGameButtons)
            {
                std::cout << "Load a Game: Not implemented today\n";
            }
            else if (clickAction && i==BUTTON_START && currentMenu == &newGameButtons)
            {
                creatNewGame(menuButtons[BUTTON_USERNAME].buttonText->getString(), newGameButtons[BUTTON_SEED].buttonText->getString(), 4);
                quitMenuWindow(false);
                currentMenu = &menuButtons;
                currentText = &menuTexts;
                return true;
            }
        }

        break;

    case sf::Event::TextEntered:
        if (event.text.unicode > ASCI_BEGIN && event.text.unicode < ASCI_END)
        {
            writeChar(converter.to_bytes(event.text.unicode));
        }
        break;

    case sf::Event::KeyPressed:

        switch (event.key.code)
        {
        case sf::Keyboard::Escape:
            quitMenuWindow(true);
            return true;

        case sf::Keyboard::BackSpace:
            deleteChar();

        default:
            break;
        }

        break;

    case sf::Event::Closed:
        quitMenuWindow(true);
        return true;

    default:
        break;
    }

    return false;
}

/*!
 * \brief Add a letter to the selected button
 * @param ch letter to add
 */
void MenuWindow::writeChar(std::string ch){
    for (unsigned i = 0; i < currentMenu->size(); i++)
    {
        std::string newString = currentMenu->at(i).buttonText->getString();
        if (currentMenu->at(i).redBorder && (int)((std::string)currentMenu->at(i).buttonText->getString()).size() < currentMenu->at(i).maxTextSize)
        {
            currentMenu->at(i).addChar(ch);
        }
        if (currentMenu->at(i).redBorder && i==BUTTON_PLAYER && currentMenu == &newGameButtons)
        {
            currentMenu->at(i).buttonText->setString(ch);
            currentMenu->at(i).centerText(true);
        }
    }
}

/*!
 * \brief Delete a letter to the selected button
 */
void MenuWindow::deleteChar(){

    for (auto &button : *currentMenu)
    {
        std::string newString = button.buttonText->getString();
        if (button.redBorder && newString.size() != 0 && button.maxTextSize != 0)
        {
            button.delChar();
        }
    }
}

/*!
 * \brief Load all the textures that will be display on the menu
 */
void MenuWindow::loadMenuTexture()
{

    backgroundTexture = (std::unique_ptr<TextureDisplayer>)new TextureDisplayer(RESOURCES_PATH "/img/menu/background.png");
    backgroundTexture->addSprite();
    float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(WINDOW_LENGTH));
    backgroundTexture->setHudSpritePosition(backgroundScale, WINDOW_LENGTH, WINDOW_WIDTH, 0, 0);

    menuFont = (std::unique_ptr<sf::Font>)new sf::Font();
    if (!menuFont->loadFromFile(RESOURCES_PATH "/img/hud/font.otf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }

    std::ifstream file(RESOURCES_PATH "/img/menu/menu.json");
    if (!file.is_open())
    {
        std::cerr << "Error while opening json ressources file /img/menu/menu.json" << std::endl;
        exit(1);
    }
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::unique_ptr<Json::CharReader> reader = std::unique_ptr<Json::CharReader>(Json::CharReaderBuilder().newCharReader());
    Json::Value obj;
    std::string errors;
    reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

    Json::Value &data = obj["welcomeTexts"];

    for (auto &dataMenu : data)
    {
        menuTexts.emplace_back(dataMenu["text"].asString(), *menuFont, dataMenu["size"].asInt());
        if (dataMenu["bold"].asBool())
        {
            menuTexts.back().setStyle(sf::Text::Bold);
        }
        menuTexts.back().setFillColor(sf::Color::Black);
        menuTexts.back().setPosition((int)(WINDOW_LENGTH - dataMenu["xOffset"].asFloat() * menuTexts.back().getLocalBounds().height - menuTexts.back().getLocalBounds().width), 
                                    (int)(WINDOW_WIDTH - dataMenu["yOffset"].asFloat() * menuTexts.back().getLocalBounds().height));
    }

    data = obj["newGameTexts"];

    for (auto &dataMenu : data)
    {
        newGameTexts.emplace_back(dataMenu["text"].asString(), *menuFont, dataMenu["size"].asInt());
        if (dataMenu["bold"].asBool())
        {
            newGameTexts.back().setStyle(sf::Text::Bold);
        }
        newGameTexts.back().setFillColor(sf::Color::Black);
        newGameTexts.back().setPosition((int)(WINDOW_LENGTH - dataMenu["xOffset"].asFloat() * newGameTexts.back().getLocalBounds().height - newGameTexts.back().getLocalBounds().width), 
                                        (int)(WINDOW_WIDTH - dataMenu["yOffset"].asFloat() * newGameTexts.back().getLocalBounds().height));
    }
    
    data = obj["welcomeButtons"];

    for (auto &dataMenu : data)
    {
        menuButtons.emplace_back(   sf::Vector2f(dataMenu["width"].asFloat() * menuTexts[0].getLocalBounds().width, dataMenu["height"].asFloat() * menuTexts[0].getLocalBounds().height), 
                                    sf::Vector2f(   menuTexts[0].getPosition().x + dataMenu["x"].asFloat() * menuTexts[0].getLocalBounds().width, 
                                                    menuTexts[0].getPosition().y - dataMenu["y"].asFloat() * menuTexts[0].getLocalBounds().height), buttonColor);
        menuButtons.back().setText(40, sf::Vector2f(0, 0), dataMenu["text"].asString(), *menuFont, dataMenu["sizeMax"].asInt());
    }

    newGameButtons.push_back(menuButtons[BUTTON_USERNAME]);
    newGameButtons.push_back(menuButtons[BUTTON_SERVER]);
    newGameButtons.push_back(menuButtons[BUTTON_PORT]);

    data = obj["newGameButtons"];

    for (auto &dataMenu : data)
    {
        newGameButtons.emplace_back( sf::Vector2f(dataMenu["width"].asFloat() * menuTexts[0].getLocalBounds().width, dataMenu["height"].asFloat() * menuTexts[0].getLocalBounds().height), 
                                    sf::Vector2f(   menuTexts[0].getPosition().x + dataMenu["x"].asFloat() * menuTexts[0].getLocalBounds().width, 
                                                    menuTexts[0].getPosition().y - dataMenu["y"].asFloat() * menuTexts[0].getLocalBounds().height), buttonColor);
        newGameButtons.back().setText(40, sf::Vector2f(0, 0), dataMenu["text"].asString(), *menuFont, dataMenu["sizeMax"].asInt());
    }
}

/*!
 * \brief Function that deteck where the user click and what to send to the engine
 * @param timeSecond is a boolean used to 
 */
long MenuWindow::getCurrentTime(bool timeSecond)
{
    if (timeSecond)
    {
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    } 
    else 
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
}

}
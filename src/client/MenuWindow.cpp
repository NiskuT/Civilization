#include <client.hpp>
#include <iostream>
#include <json/json.h>
#include <fstream>
#include <string>
#include <codecvt>

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

#define CREATE_GAME "new"

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
    currentMenu = &menuButtons;
    currentText = &menuTexts;
}

/*!
 * \brief Display all the menu on the screen
 */
void MenuWindow::displayWindow()
{

    gameEnginePtr->clientWindow->clear(sf::Color::Blue);

    gameEnginePtr->clientWindow->draw(backgroundTexture->getSprite());

    for (unsigned i = 0; i < currentMenu->size(); i++)
    {
        gameEnginePtr->clientWindow->draw(*currentMenu->at(i).buttonRect);
        gameEnginePtr->clientWindow->draw(*currentMenu->at(i).buttonText);
    }

    for (unsigned i = 0; i < currentText->size(); i++)
    {
        gameEnginePtr->clientWindow->draw(currentText->at(i));
    }
    
    gameEnginePtr->clientWindow->display();
}

/*!
 * \brief Loop that look for events to happend and call displayWindow()
 * @param clientWindow is window that comes from the engine
 * @param quitGame is the function used to quit the menu, it is load as an attribut
 */
void MenuWindow::startMenu()
{
    loadMenuTexture();
    if (gameEnginePtr == nullptr)
    {
        return;
    }

    long lastUpdateTimer = getCurrentTime(false);

    while (gameEnginePtr->clientWindow->isOpen())
    {

        if (getCurrentTime(false) - lastUpdateTimer > (REFRESH_TIME))
        {
            displayWindow();
            lastUpdateTimer = getCurrentTime(false);
        }

        // handle events
        sf::Event event;
        while (gameEnginePtr->clientWindow->pollEvent(event))
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
    bool isOnButton = false;

    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:

        clickPoint = sf::Mouse::getPosition(*gameEnginePtr->clientWindow);

        for (unsigned i = 0; i < currentMenu->size(); i++)
        {
            if (clickAction(clickPoint, i, isOnButton))
            {
                return false;
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
            gameEnginePtr->handleQuitMenu(true);
            return true;

        case sf::Keyboard::K:
            gameEnginePtr->handleQuitMenu(false);
            return true;

        case sf::Keyboard::BackSpace:
            deleteChar();

        default:
            break;
        }

        break;

    case sf::Event::Closed:
        gameEnginePtr->handleQuitMenu(true);
        return true;

    default:
        break;
    }

    return false;
}

/*!
 * \brief Do the action corresponding to a click on a particular button
 * @param clickPoint cursor coordonate
 * @param index index of the testing button
 */
bool MenuWindow::clickAction(sf::Vector2i clickPoint, int index, bool isOnButton)
{
    currentMenu->at(index).setInactive();

    if (gameEnginePtr->intersectPointRect(  
        clickPoint, 
        currentMenu->at(index).buttonRect->getGlobalBounds()))
    {
        isOnButton = currentMenu->at(index).clickButton();
    }
    if (isOnButton && index==BUTTON_CREAT && currentMenu == &menuButtons)
    {
        currentMenu = &newGameButtons;
        currentText = &newGameTexts;
    }
    else if (isOnButton && index==BUTTON_CONNECT 
                        && currentMenu == &menuButtons)
    {
        return connectToGame(menuButtons[BUTTON_ID]());
    }
    else if (isOnButton && index==BUTTON_LOAD 
                        && currentMenu == &newGameButtons)
    {
        std::cout << "Load a Game: Not implemented today\n";
    }
    else if (isOnButton && index==BUTTON_START 
                        && currentMenu == &newGameButtons)
    {
        return connectToGame(CREATE_GAME);
    }
    return false;
}

/*!
 * \brief connect to the server
 * @param gameID id of the new Game
 */
bool MenuWindow::connectToGame(std::string gameID)
{
    bool isConnected = gameEnginePtr->tryConnection(
        gameID,
        menuButtons[BUTTON_USERNAME](), 
        menuButtons[BUTTON_SERVER](), 
        menuButtons[BUTTON_PORT]());
    
    if (isConnected)
    {
        gameEnginePtr->handleQuitMenu(false);
        currentMenu = &menuButtons;
        currentText = &menuTexts;
        return true;
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
        std::string newString = currentMenu->at(i)();
        if (currentMenu->at(i).redBorder 
            && (int)((std::string)currentMenu->at(i)()).size() < currentMenu->at(i).maxTextSize)
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
        std::string newString = button();
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
    float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(gameEnginePtr->clientWindow->getSize().x ));
    backgroundTexture->setHudSpritePosition(backgroundScale, gameEnginePtr->clientWindow->getSize().x , gameEnginePtr->clientWindow->getSize().y , 0, 0);

    if (!menuFont.loadFromFile(RESOURCES_PATH "/img/hud/font.otf"))
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

    loadText(obj["welcomeTexts"]);
    loadButton(obj["welcomeButtons"]);

    currentMenu = &newGameButtons;
    currentText = &newGameTexts;
    
    newGameButtons.push_back(menuButtons[BUTTON_USERNAME]);
    newGameButtons.push_back(menuButtons[BUTTON_SERVER]);
    newGameButtons.push_back(menuButtons[BUTTON_PORT]);
    
    loadText(obj["newGameTexts"]);
    loadButton(obj["newGameButtons"]);

    currentMenu = &menuButtons;
    currentText = &menuTexts;
}

void MenuWindow::loadText(Json::Value &data)
{
    for (auto &dataMenu : data)
    {
        currentText->emplace_back(dataMenu["text"].asString(), menuFont, dataMenu["size"].asInt());
        if (dataMenu["bold"].asBool())
        {
            currentText->back().setStyle(sf::Text::Bold);
        }
        currentText->back().setFillColor(sf::Color::Black);
        currentText->back().setPosition((int)(gameEnginePtr->clientWindow->getSize().x 
                                                - dataMenu["xOffset"].asFloat() 
                                                * currentText->back().getLocalBounds().height 
                                                - currentText->back().getLocalBounds().width), 

                                        (int)(gameEnginePtr->clientWindow->getSize().y
                                                - dataMenu["yOffset"].asFloat() 
                                                * currentText->back().getLocalBounds().height));
    }
}

void MenuWindow::loadButton(Json::Value &data)
{
    for (auto &dataMenu : data)
    {
        currentMenu->emplace_back(  sf::Vector2f(dataMenu["width"].asFloat() 
                                                    * menuTexts[0].getLocalBounds().width, 
                                                 dataMenu["height"].asFloat() 
                                                    * menuTexts[0].getLocalBounds().height), 
                                                    
                                    sf::Vector2f(   menuTexts[0].getPosition().x 
                                                    + dataMenu["x"].asFloat() 
                                                    * menuTexts[0].getLocalBounds().width, 
                                                    menuTexts[0].getPosition().y 
                                                    - dataMenu["y"].asFloat() 
                                                    * menuTexts[0].getLocalBounds().height), 
                                                    buttonColor);

        currentMenu->back().setText(40, sf::Vector2f(0, 0), dataMenu["text"].asString(), menuFont, dataMenu["sizeMax"].asInt());
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
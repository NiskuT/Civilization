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

#define QUIT_OFFSET 10
#define QUIT_SCALE 0.09375

#define CREATE_GAME "new"

#define ASCI_BEGIN 41
#define ASCI_END 123

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

const sf::Color buttonColor = sf::Color(247, 200, 195, 255);

using namespace client;

/*!
 * @brief Constructor
 *
 * Constructor of MenuWindow class
 */
MenuWindow::MenuWindow()
{
    currentMenu = &menuButtons;
    currentText = &menuTexts;
}

/*!
 * @brief Display all the menu on the screen
 */
void MenuWindow::displayWindow()
{

    gameEnginePtr->clientWindow->clear(sf::Color::Blue);

    gameEnginePtr->clientWindow->draw(backgroundTexture->getSprite());
    gameEnginePtr->clientWindow->draw(quitTexture->getSprite());

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
 * @brief Loop that look for events to happend and call displayWindow()
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
 * @brief Test events and do actions corresponding to the event
 * @param event pointer to the event
 */
bool MenuWindow::menuEventHappened(sf::Event &event)
{

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
            gameEnginePtr->handleQuitMenu(true);
            return true;

        case sf::Keyboard::K:
            gameEnginePtr->handleQuitMenu(false);
            return true;

        case sf::Keyboard::BackSpace:
            deleteChar();
            break;

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
 * @brief Do the action corresponding to a click on a particular button
 * @param clickPoint cursor coordonate
 * @param index index of the testing button
 * @param isOnButton check if the button is active
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
    if (gameEnginePtr->intersectPointRect( 
        clickPoint, 
        quitTexture->getSprite().getGlobalBounds()))
    {
        gameEnginePtr->handleQuitMenu(true);
        return true;
    }
    if (isOnButton && index == BUTTON_CREAT && currentMenu == &menuButtons)
    {
        currentMenu = &newGameButtons;
        currentText = &newGameTexts;
    }
    else if (isOnButton && index == BUTTON_CONNECT && currentMenu == &menuButtons)
    {
        return connectToGame(menuButtons[BUTTON_ID]());
    }
    else if (isOnButton && index == BUTTON_LOAD && currentMenu == &newGameButtons)
    {
        std::cout << "Load a Game: Not implemented today\n";
    }
    else if (isOnButton && index == BUTTON_START && currentMenu == &newGameButtons)
    {
        return connectToGame(CREATE_GAME);
    }
    return false;
}

/*!
 * @brief connect to the server
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
        std::cout << "Change\n";
        gameEnginePtr->handleQuitMenu(false);
        currentMenu = &menuButtons;
        currentText = &menuTexts;
        return true;
    }
    return false;
}

/*!
 * @brief Add a letter to the selected button
 * @param ch letter to add
 */
void MenuWindow::writeChar(std::string ch)
{
    for (unsigned i = 0; i < currentMenu->size(); i++)
    {
        std::string newString = currentMenu->at(i)();
        if (currentMenu->at(i).redBorder &&
            (int)((std::string)currentMenu->at(i)()).size() < currentMenu->at(i).maxTextSize)
        {
            currentMenu->at(i).addChar(ch);
        }
        if (currentMenu->at(i).redBorder && i == BUTTON_PLAYER && currentMenu == &newGameButtons)
        {
            currentMenu->at(i).buttonText->setString(ch);
            currentMenu->at(i).centerText(true);
        }
    }
}

/*!
 * @brief Delete a letter to the selected button
 */
void MenuWindow::deleteChar()
{

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
 * @brief Load all the textures that will be display on the menu
 */
void MenuWindow::loadMenuTexture()
{

    backgroundTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/menu/background.png");
    backgroundTexture->addSprite();
    float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(gameEnginePtr->clientWindow->getSize().x));
    backgroundTexture->setHudSpritePosition(backgroundScale, gameEnginePtr->clientWindow->getSize().x, gameEnginePtr->clientWindow->getSize().y, 0, 0);

    quitTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/hud/leave.png");
    quitTexture->addSprite();
    quitTexture->moveSpritePosition(QUIT_OFFSET, QUIT_OFFSET);
    quitTexture->getSprite().setScale(QUIT_SCALE, QUIT_SCALE);


    if (!menuFont.loadFromFile(RESOURCES_PATH "/hud/font.otf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }

    std::ifstream file(RESOURCES_PATH "/menu/menu.json");
    if (!file.is_open())
    {
        std::cerr << "Error while opening json ressources file /menu/menu.json" << std::endl;
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

/*!
 * @brief Load all the text of a Button from a JSon
 * @param data where the data is stored
 */
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
        currentText->back().setPosition(setXAxisButtonTextPosition(dataMenu["xOffset"].asFloat()),
                                        setYAxisButtonTextPosition(dataMenu["yOffset"].asFloat()));
    }
}

/*!
 * @brief Load all the button from a JSon
 * @param data where the data is stored
 */
void MenuWindow::loadButton(Json::Value &data)
{
    for (auto &dataMenu : data)
    {
        currentMenu->emplace_back(setButtonSize(dataMenu["width"].asFloat(), dataMenu["height"].asFloat()),
                                  setButtonPosition(dataMenu["x"].asFloat(), dataMenu["y"].asFloat()),
                                  buttonColor);

        currentMenu->back().setText(40, sf::Vector2f(0, 0), dataMenu["text"].asString(), menuFont, dataMenu["sizeMax"].asInt());
    }
}

/*!
 * @brief Calcul the x offset of the text of a button
 * @param offset x offset of the text
 */
int MenuWindow::setXAxisButtonTextPosition(float offset)
{
    return (int)(gameEnginePtr->clientWindow->getSize().x 
        - offset 
        * currentText->back().getLocalBounds().height 
        - currentText->back().getLocalBounds().width);
}

/*!
 * @brief Calcul the y offset of the text of a button
 * @param offset y offset of the text
 */
int MenuWindow::setYAxisButtonTextPosition(float offset)
{
    return (int)(gameEnginePtr->clientWindow->getSize().y 
        - offset 
        * currentText->back().getLocalBounds().height);
}

/*!
 * @brief Calcul the size of a button
 * @param width value comming from the JSon file
 * @param height value comming from the JSon file
 */
sf::Vector2f MenuWindow::setButtonSize(float width, float height)
{
    return sf::Vector2f(width * menuTexts[0].getLocalBounds().width,
                        height * menuTexts[0].getLocalBounds().height);
}

/*!
 * @brief Calcul the position of a button
 * @param x value comming from the JSon file
 * @param y value comming from the JSon file
 */
sf::Vector2f MenuWindow::setButtonPosition(float x, float y)
{
    return sf::Vector2f(menuTexts[0].getPosition().x + x * menuTexts[0].getLocalBounds().width,
                        menuTexts[0].getPosition().y - y * menuTexts[0].getLocalBounds().height);
}

/*!
 * @brief Function that deteck where the user click and what to send to the engine
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

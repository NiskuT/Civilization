#include <client.hpp>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <cmath>
#include <string>
#include <codecvt>
#include <variant>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50

#define NUMBER_OF_FIELD 12

#define TCHAT_MAX_SIZE 200

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define ACTION_CARD_PROPORTION 0.125
#define NBR_CHAR_MAX_PER_LIGNE 22
#define TURN_NUMBER 2

#define ASCI_BEGIN 19
#define ASCI_END 127

#define CARD_BORDER 18

#define INDEX_CHAT_BUTTON 8
#define INDEX_MAP_BUTTON 9
#define INDEX_QUIT_BUTTON 10

#define ARROW_INDEX 5

#define ELEMENT_PATH "/map/element/"
#define CHAT_MIN_SIZE 7

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

typedef std::variant<shared::Caravan, shared::Barbarian, shared::BarbarianVillage, shared::ControlPawn, shared::City> variantElement;

const std::vector<sf::Color> PLAYER_COLOR = {sf::Color(119, 238, 217, 160), sf::Color(251, 76, 255, 160), sf::Color(93, 109, 126, 160), sf::Color(230, 176, 170, 160)};
const sf::Color TEXT_COLOR = sf::Color(240, 230, 230);
const sf::Color TEXT_FOR_USER_BUTTON_COLOR = sf::Color(255, 255, 255, 100);
const sf::Color TEXT_FOR_USER_COLOR = sf::Color(204, 0, 102);
const sf::Color END_OF_TURN_BUTTON_COLOR = sf::Color(247, 200, 195);

using namespace client;

/*!
 * @brief Constructor
 *
 * Constructor of GameWindow class
 */
GameWindow::GameWindow()
{
    firstHexagonPosition = {MAP_X_OFFSET, MAP_Y_OFFSET};
    chatBox = std::make_unique<Chat>();

    const Json::Value &data = openJsonFile("/pop-up/dataButton.json");
    validateBoxesWindow = std::make_unique<PopUpWindow>(WINDOW_LENGTH, WINDOW_WIDTH, data, true);
    winnerWindow = std::make_unique<PopUpWindow>(WINDOW_LENGTH, WINDOW_WIDTH, data, false);
    validateBoxesWindow->gameWindow = this;

    chatBox = std::make_unique<Chat>();
}

/*!
 * @brief Display all the different variable in the screen
 */
void GameWindow::displayWindow()
{

    gameEnginePtr->clientWindow->clear(sf::Color::Blue);

    backgroundTexture->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);

    for (auto &mapTexture : mapTextureToDisplay)
    {
        mapTexture.drawTextureDisplayerSprite(gameEnginePtr->clientWindow);
    }

    std::unique_lock<std::mutex> lock(updatePlayerMutex);
    for (auto &elementTexture : elementTextureToDisplay)
    {
        elementTexture.second->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);
    }
    lock.unlock();

    for (auto &priorityCardTexture : priorityCards)
    {
        priorityCardTexture.texture->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);
        gameEnginePtr->clientWindow->draw(*priorityCardTexture.title);
        gameEnginePtr->clientWindow->draw(*priorityCardTexture.nbOfBoxesText);
        if (priorityCardTexture.isUp)
        {
            gameEnginePtr->clientWindow->draw(*priorityCardTexture.body);
            priorityCardTexture.validateButton->drawButton(gameEnginePtr->clientWindow);
        }
    }
    boxTexture->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);

    for (auto &actionCardTexture : actionCardsToDisplay)
    {
        gameEnginePtr->clientWindow->draw(actionCardTexture.texture->getSprite(0));
        gameEnginePtr->clientWindow->draw(*actionCardTexture.title);
        gameEnginePtr->clientWindow->draw(*actionCardTexture.body);
    }

    for (auto &whoIsPlayingButton : whoIsPlayingButtons)
    {
        whoIsPlayingButton.drawButton(gameEnginePtr->clientWindow);
    }

    gameEnginePtr->clientWindow->draw(hudTextureToDisplay.at(TURN_NUMBER % 5).getSprite());

    for (auto &hudTexture : hudTextureToDisplay)
    {
        hudTexture.drawTextureDisplayerSprite(gameEnginePtr->clientWindow);
    }

    textForTheUser->drawButton(gameEnginePtr->clientWindow);
    endOfRoundButton->drawButton(gameEnginePtr->clientWindow);

    if (validateBoxesWindow->isWindowActive)
    {
        validateBoxesWindow->drawValidateBoxesButtons(gameEnginePtr->clientWindow);
    }

    if (isChatOpen)
    {
        chatBox->drawChat(gameEnginePtr->clientWindow);
    }

    if (winnerWindow->isWindowActive)
    {
        winnerWindow->drawWinnerWindow(gameEnginePtr->clientWindow);
    }

    gameEnginePtr->clientWindow->display();
}

/*!
 * @brief Loop that look for events to happend and call displayWindow()
 * @param clientWindow is window that comes from the engine
 * @param quitGame is the function used to quit the menu, it is load as an attribut
 * @param callback is the function used to return where the user click on the screen
 */
void GameWindow::startGame()
{
    if (gameEnginePtr == nullptr)
    {
        return;
    }

    loadMapTexture();
    loadElementTexture();
    updateElementTexture();
    loadHudTexture();
    gameEnginePtr->areTextureLoaded.store(true);
    addPlayer(gameEnginePtr->myself->getName());

    std::shared_ptr<bool> moveMode = std::make_shared<bool>(false);
    std::shared_ptr<bool> clickMode = std::make_shared<bool>(false);

    sf::Vector2i clickStartingPoint;

    long lastUpdateTimer = getCurrentTime(false);

    while (gameEnginePtr->clientWindow->isOpen())
    {

        if (getCurrentTime(false) - lastUpdateTimer > (100 / 3))
        {
            displayWindow();
            lastUpdateTimer = getCurrentTime(false);
        }

        // handle events
        sf::Event event;
        while (gameEnginePtr->clientWindow->pollEvent(event))
        {
            if (handleGameEvent(event, clickStartingPoint, moveMode, clickMode))
            {
                return;
            }
        }
    }
}

/*!
 * @brief Test events and do actions corresponding to the event
 * @param event pointer to the event
 * @param clickStartingPoint reference used to know where the user start pressing mouse
 * @param moveMode pointer to know if the map is moving on the screen
 * @param clickMode pointer to know if the user is clicking on the screen
 */
bool GameWindow::handleGameEvent(sf::Event &event, sf::Vector2i &clickStartingPoint, std::shared_ptr<bool> moveMode, std::shared_ptr<bool> clickMode)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:

        *clickMode = true;
        clickStartingPoint = sf::Mouse::getPosition(*gameEnginePtr->clientWindow);
        if (clickAction(event, clickStartingPoint, moveMode))
        {
            std::cout << "You have quit the game\n";
            gameEnginePtr->handleQuitMenu(true);
            return true;
        }
        break;

    case sf::Event::MouseButtonReleased:
        *clickMode = false;
        break;

    case sf::Event::MouseMoved:
        if (*moveMode && *clickMode)
        {
            moveMap(clickStartingPoint, sf::Mouse::getPosition(*gameEnginePtr->clientWindow));
        }
        break;

    case sf::Event::TextEntered:
        if (event.text.unicode > ASCI_BEGIN && event.text.unicode < ASCI_END && isChatOpen)
        {
            chatBox->addChatChar(converter.to_bytes(event.text.unicode));
        }
        break;

    case sf::Event::KeyPressed:
        if (handleKeyboardEvent(event.key))
            return true;
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
 * @brief Test keyboard events and do actions corresponding to the event
 * @param keyEvent The event
 * @param moveMode pointer to know if the map is moving on the screen
 * @param clickStartingPoint reference used to know where the user start pressing mouse
 */
bool GameWindow::handleKeyboardEvent(sf::Event::KeyEvent keyEvent)
{
    switch (keyEvent.code)
    {
    case sf::Keyboard::Enter:
        if (isChatOpen)
        {
            sendMessage();
        }
        break;

    case sf::Keyboard::BackSpace:
        chatBox->deleteChatChar();
        break;

    default:
        break;
    }
    return false;
}

/*!
 * @brief This function send a message to the server
 */
void GameWindow::sendMessage()
{
    std::unique_lock<std::mutex> lock(chatBox->mutexChat);
    std::string message = "chat " + chatBox->message + "\n";
    lock.unlock();

    if (message.size() < CHAT_MIN_SIZE)
        return;

    std::unique_lock<std::mutex> lock2(gameEnginePtr->myself->qAndA.sharedDataMutex);
    gameEnginePtr->myself->qAndA.question = message;
    lock2.unlock();
    gameEnginePtr->askServer();
}

/*!
 * @brief Change the cursor type to a hand or an arrow
 * @param moveMode pointer to know if the map is moving on the screen
 */
void GameWindow::changeMouseCursor(sf::Event &event, std::shared_ptr<bool> moveMode)
{
    if (event.mouseButton.button == sf::Mouse::Right)
    {
        sf::Vector2i nullPosition(0, 0);
        moveMap(nullPosition, {MAP_X_OFFSET, MAP_Y_OFFSET}, true);
        gameEnginePtr->clientWindow->setMouseCursor(clientCursor);
        return;
    }

    if (*moveMode)
    {
        *moveMode = false;
        clientCursor.loadFromSystem(sf::Cursor::Arrow);
    }
    else
    {
        *moveMode = true;
        clientCursor.loadFromSystem(sf::Cursor::Hand);
    }
    gameEnginePtr->clientWindow->setMouseCursor(clientCursor);
}

/*!
 * @brief Move the map on the screen
 * @param clickStartingPoint reference used to know where the user start pressing mouse
 * @param position position of the mouse
 * @param reset if true, reset the map to the original position, false by default
 */
void GameWindow::moveMap(sf::Vector2i &clickStartingPoint, sf::Vector2i position, bool reset)
{
    if (reset)
    {
        clickStartingPoint.x = firstHexagonPosition[0];
        clickStartingPoint.y = firstHexagonPosition[1];
    }
    std::array<int, 2> newMapOffset = {position.x - clickStartingPoint.x,
                                       position.y - clickStartingPoint.y};

    if (reset)
    {
        firstHexagonPosition = {MAP_X_OFFSET, MAP_Y_OFFSET};
    }
    else
    {
        firstHexagonPosition = {firstHexagonPosition[0] + newMapOffset[0],
                                firstHexagonPosition[1] + newMapOffset[1]};
    }

    for (unsigned i = 0; i < mapTextureToDisplay.size(); i++)
    {
        mapTextureToDisplay[i].moveSpritePosition(newMapOffset[0], newMapOffset[1]);
    }

    for (auto &kv : elementTextureToDisplay)
    {
        kv.second->moveSpritePosition(newMapOffset[0], newMapOffset[1]);
    }

    clickStartingPoint = sf::Mouse::getPosition(*gameEnginePtr->clientWindow);
}

/*!
 * @brief Open JSON File
 * @param path path of the JSON File
 */
const Json::Value GameWindow::openJsonFile(std::string path)
{
    std::ifstream file(RESOURCES_PATH + path);

    if (!file.is_open())
    {
        std::cerr << "Error while opening json ressources file" << std::endl;
        std::cerr << path << std::endl;
        exit(1);
    }
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::unique_ptr<Json::CharReader> reader = std::unique_ptr<Json::CharReader>(Json::CharReaderBuilder().newCharReader());
    Json::Value obj;
    std::string errors;
    reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

    const Json::Value &data = obj["data"];

    return data;
}

/*!
 * @brief Move to right priority cards when a player play one
 * @param difficulty level of difficulty when the card is played (0 to 4 for the 5 different field)
 */
void GameWindow::moveToRightPriorityCards(int difficulty)
{
    const Json::Value &dataNumber = openJsonFile("/hud/data-number.json");

    int xPos;
    int yPos;

    for (unsigned i = difficulty; i > 0; i--)
    {
        priorityCards[i - 1].difficulty = i;
        std::iter_swap(priorityCards.begin() + i, priorityCards.begin() + (i - 1));
    }
    priorityCards[0].difficulty = 0;

    for (int i = 0; i <= difficulty; i++)
    {
        xPos = dataNumber["priority-card-offset"].asFloat() * WINDOW_LENGTH * i + dataNumber["priority-card-first-offset"].asFloat() * WINDOW_LENGTH;
        yPos = priorityCards[i].texture->getSprite().getPosition().y;
        priorityCards[i].texture->getSprite().setPosition(xPos, yPos);
        priorityCards[i].movePriorityCardElements(dataNumber);
    }
}

/*!
 * @brief Detect when we click on a priority card or on the play button on priorityCard and make the action associated
 * @param cursorRect emplacement of the mouse
 * Example to use the winner window in this function:
 * setWinnerWindow("Lasso", "1. Tech-Wheel level >=24 \n2. More than 15 control pawns \n3. You are the best");
 */
bool GameWindow::priorityCardClickAction(sf::Vector2i clickPosition)
{
    std::string questionString;
    std::string nbOfBoxesOnPriorityCard;
    int newNumberOfBoxes;

    sf::FloatRect spriteArrowMoreBoxes = validateBoxesWindow->arrowMoreTexture->getSprite().getGlobalBounds();
    sf::FloatRect spriteArrowLessBoxes = validateBoxesWindow->arrowLessTexture->getSprite().getGlobalBounds();
    sf::FloatRect spriteValidateBoxesButton = validateBoxesWindow->doneTexture->getSprite().getGlobalBounds();

    // If we click on the done button to accept the number of boxes to play
    if (gameEnginePtr->intersectPointRect(clickPosition, spriteValidateBoxesButton) && validateBoxesWindow->isWindowActive)
    {
        validateBoxesWindow->isWindowActive = false;
        moveToRightPriorityCards(validateBoxesWindow->priorityCardPlayed);
        newNumberOfBoxes = validateBoxesWindow->nbOfBoxesMax - validateBoxesWindow->nbOfBoxesChosen;
        validateBoxesWindow->nbOfBoxesMax = newNumberOfBoxes;
        priorityCards[0].nbOfBoxesText->setString(std::to_string(newNumberOfBoxes) + " x");

        gameEnginePtr->handlePriorityCardPlay(
            validateBoxesWindow->priorityCardPlayedType,
            validateBoxesWindow->priorityCardPlayed,
            validateBoxesWindow->nbOfBoxesChosen);
            
            return true;
    }

    // if we click on the little arrow to add boxes
    if (gameEnginePtr->intersectPointRect(clickPosition, spriteArrowMoreBoxes) &&
        validateBoxesWindow->isWindowActive &&
        validateBoxesWindow->nbOfBoxesChosen < validateBoxesWindow->nbOfBoxesMax)
    {
        validateBoxesWindow->nbOfBoxesChosen++;
        validateBoxesWindow->chooseNumberOfBoxesButton->buttonText->setString(std::to_string(validateBoxesWindow->nbOfBoxesChosen)); // sent by the server
        return true;
    }

    // if we click on the little arrow to delete boxes
    if (gameEnginePtr->intersectPointRect(clickPosition, spriteArrowLessBoxes) &&
        validateBoxesWindow->isWindowActive &&
        validateBoxesWindow->nbOfBoxesChosen > 0)
    {
        validateBoxesWindow->nbOfBoxesChosen--;
        validateBoxesWindow->chooseNumberOfBoxesButton->buttonText->setString(std::to_string(validateBoxesWindow->nbOfBoxesChosen)); // sent by the server
        return true;
    }

    for (auto &priorityCard : priorityCards)
    {
        sf::FloatRect spriteCards = priorityCard.texture->getSprite().getGlobalBounds();
        sf::FloatRect spriteValidateButton = priorityCard.validateButton->buttonRect->getGlobalBounds();

        // if we click on the play button on priorityCards
        if (gameEnginePtr->intersectPointRect(clickPosition, spriteValidateButton) && priorityCard.isUp)
        {
            validateBoxesWindow->isWindowActive = true;
            validateBoxesWindow->priorityCardPlayed = priorityCard.difficulty;
            validateBoxesWindow->priorityCardPlayedType = priorityCard.type;

            nbOfBoxesOnPriorityCard = priorityCard.nbOfBoxesText->getString().substring(0, 1);
            validateBoxesWindow->nbOfBoxesChosen = std::stoi(nbOfBoxesOnPriorityCard); // sent by the server
            validateBoxesWindow->nbOfBoxesMax = std::stoi(nbOfBoxesOnPriorityCard);    // sent by the server

            validateBoxesWindow->chooseNumberOfBoxesButton->buttonText->setString(nbOfBoxesOnPriorityCard); // sent by the server
            questionString = "You have " + nbOfBoxesOnPriorityCard + " boxes \nHow many boxes do you want to play?";
            validateBoxesWindow->title->setString(questionString);
            return true;
        }

        // if we click on a priorityCard card
        if (gameEnginePtr->intersectPointRect(clickPosition, spriteCards))
        {
            priorityCard.moveUpPriorityCard();
            return true;
        }
    }

    return false;
}

/*!
 * @brief Function that deteck where the user click and what to send to the engine
 * @param clickPosition is the position on the cursor when the user click
 * @brief Dectect click and actions to do after
 */
bool GameWindow::clickAction(sf::Event &event, sf::Vector2i clickPosition, std::shared_ptr<bool> moveMode)
{
    if (!*moveMode)
    {
        if (priorityCardClickAction(clickPosition))
        {
            return false;
        }

        if (onHexagonClick(clickPosition))
        {
            return false;
        }
    }

    if (gameEnginePtr->intersectPointRect(clickPosition, endOfRoundButton->buttonRect->getGlobalBounds()))
    {
        gameEnginePtr->handleEndTurnButton();
        return false;
    }

    // Check if the click position is inside the move map button
    if (gameEnginePtr->intersectPointRect(clickPosition, hudTextureToDisplay[INDEX_MAP_BUTTON].getSprite().getGlobalBounds()))
    {
        changeMouseCursor(event, moveMode);
        return false;
    }

    // Check if the click position is inside the chat button
    if (gameEnginePtr->intersectPointRect(clickPosition, hudTextureToDisplay[INDEX_CHAT_BUTTON].getSprite().getGlobalBounds()))
    {
        isChatOpen = !isChatOpen;
        return false;
    }

    // Check if the click position is inside the chat button
    if (gameEnginePtr->intersectPointRect(clickPosition, hudTextureToDisplay[INDEX_QUIT_BUTTON].getSprite().getGlobalBounds()))
    {
        return true;
    }
    return false;
}

void GameWindow::rotateTechWheel(int newLevel)
{
    int newRotation = techWheelRotation[newLevel];
    hudTextureToDisplay[ARROW_INDEX].getSprite(0).setRotation(newRotation);
}

/*!
 * @brief Display text on the cards
 * @param cards pointer to the card you want to setUp the text
 * @param title text to be display on the top of the card
 * @param body text to be display on body of the card, float
 * @param titleFont Font that will be used for the titile of the card
 * @param bodyFont Font that will be used for the body of the card
 * @param titleTextSizeProportion Proportion of the title
 * @param bodyTextSizeProportion Proportion of the body
 */
void GameWindow::setUpText(
    GraphicCard &card,
    std::string title,
    std::string body,
    sf::Font &titleFont,
    sf::Font &bodyFont,
    const Json::Value &dataNumber,
    float titleTextProportion,
    float bodyTextProportion)
{
    int titleTextSize = titleTextProportion * WINDOW_LENGTH;
    int bodyTextSize = bodyTextProportion * WINDOW_LENGTH;

    // display the title on the card
    card.title = std::make_unique<sf::Text>(title, titleFont, titleTextSize);
    card.title->setStyle(sf::Text::Bold);
    card.title->setFillColor(TEXT_COLOR);
    auto titleSize = card.title->getLocalBounds();
    int xTitleOffset = (card.texture->getWidth() - titleSize.width) / 2;
    int xTitlePosition = card.texture->getSprite().getPosition().x + xTitleOffset;
    int yTitlePosition = card.texture->getSprite().getPosition().y;
    card.title->setPosition(xTitlePosition, yTitlePosition);

    // display the body on the card
    card.body = std::make_unique<sf::Text>(body, bodyFont, bodyTextSize);

    // to have the text on several lines without exceeding the card
    int countEndLine = 1;
    while (card.body->getLocalBounds().width > card.texture->getWidth() - CARD_BORDER) // 18 to not touch the black border
    {

        for (int i = countEndLine * NBR_CHAR_MAX_PER_LIGNE; i > 0; i--)
        {
            if ((char)body[i] == ' ')
            {
                body.replace(i, 1, "\n");
                countEndLine++;
                break;
            }
        }
        card.body->setString(body);
    }

    card.body->setFillColor(TEXT_COLOR);
    card.body->setLineSpacing(dataNumber["body-line-space"].asFloat());
    int xBodyOffset = dataNumber["body-x-proportion"].asFloat() * WINDOW_LENGTH;
    int yBodyOffset = dataNumber["body-y-proportion"].asFloat() * WINDOW_WIDTH;
    int xBodyPosition = card.texture->getSprite().getPosition().x + xBodyOffset;
    int yBodyPosition = card.texture->getSprite().getPosition().y + yBodyOffset;
    card.body->setPosition(xBodyPosition, yBodyPosition);
}

bool GameWindow::onHexagonClick(sf::Vector2i clickPosition)
{
    bool isClickable = false;
    std::array<int, 2> hexagonOnClick = {0, 0};
    int minimumDistance = WINDOW_LENGTH;

    for (auto &mapTexture : mapTextureToDisplay)
    {
        for (unsigned j = 0; j < mapTexture.getSize(); j++)
        {
            if (validateBoxesWindow->isWindowActive || !gameEnginePtr->intersectPointRect(clickPosition, mapTexture.getSprite(j).getGlobalBounds()))
            {
                continue;
            }
            isClickable = true;

            int x = mapTexture.getSprite(j).getGlobalBounds().left;
            int y = mapTexture.getSprite(j).getGlobalBounds().top;
            int width = mapTexture.getSprite(j).getGlobalBounds().width;
            int height = mapTexture.getSprite(j).getGlobalBounds().height;

            int distance = sqrt(pow(x + width / 2 - clickPosition.x, 2) +
                                pow(y + height / 2 - clickPosition.y, 2));

            if (distance < minimumDistance)
            {

                minimumDistance = distance;
                hexagonOnClick[1] = (int)((y - firstHexagonPosition[1])) / (int)((height * 3 / 4));
                hexagonOnClick[0] = (int)((x - firstHexagonPosition[0])) / (int)((width - 1));
            }
        }
    }

    if (isClickable)
    {
        gameEnginePtr->handleInformation(hexagonOnClick[0], hexagonOnClick[1]);
        return true;
    }
    return false;
}

/*!
 * @brief Load all the textures of the map
 */
void GameWindow::loadMapTexture()
{
    std::string hexagonImgPath = RESOURCES_PATH "/map/field/field-";
    std::array<std::string, 12> mapField = {"water", "grassland", "hill", "forest", "desert", "mountain",
                                            "wonder-everest", "wonder-galapagos", "wonder-kilimanjaro",
                                            "wonder-messa", "wonder-pantanal", "wonder-volcanic"};

    for (unsigned i{0}; i < mapField.size(); i++)
    {
        std::string mapElementPath = hexagonImgPath + mapField.at(i) + ".png";
        mapTextureToDisplay.emplace_back(mapElementPath);
    }

    for (unsigned i = 0; i < mapShared->getMapHeight(); i++)
    {
        for (unsigned j = 0; j < mapShared->getMapWidth(); j++)
        {
            int indexSprite = mapTextureToDisplay.at((int)(*mapShared)(j, i)->getFieldLevel()).getSize();
            mapTextureToDisplay.at((int)(*mapShared)(j, i)->getFieldLevel()).addSprite();
            mapTextureToDisplay.at((int)(*mapShared)(j, i)->getFieldLevel())
                .setSpritePosition(indexSprite, j, i, MAP_X_OFFSET, MAP_Y_OFFSET, {0, 0});
        }
    }
}

/*!
 * @brief Load all the textures of the elements
 */
void GameWindow::loadElementTexture()
{
    std::string folder_path = RESOURCES_PATH "/map/element/";
    std::vector<std::string> png_files;
    DIR *dir = opendir(folder_path.c_str());

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {

        if (entry->d_name[0] == '.')
        {
            continue;
        }

        std::string filename = entry->d_name;
        if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".png")
        {
            png_files.push_back(filename);
        }
    }

    closedir(dir);

    // Affiche les noms de fichiers trouvés
    for (const std::string &filename : png_files)
    {
        std::string path = RESOURCES_PATH ELEMENT_PATH + filename;
        elementTextureToDisplay[path] = std::make_unique<TextureDisplayer>(path);
    }
}

/*!
 * @brief Update all the textures of the map
 */
void GameWindow::updateElementTexture()
{
    std::lock_guard<std::mutex> lock(updatePlayerMutex);
    for (auto &kv : elementTextureToDisplay)
    {
        kv.second->clearSprites();
    }

    for (unsigned i = 0; i < mapShared->getMapHeight(); i++)
    {
        for (unsigned j = 0; j < mapShared->getMapWidth(); j++)
        {
            selectElementToDisplay(j, i);
        }
    }
}

void GameWindow::selectElementToDisplay(int x, int y)
{
    const Json::Value &elementData = openJsonFile("/map/elementPath.json");
    const Json::Value &resourceData = openJsonFile("/map/resourcePath.json");
    const Json::Value &stateCityData = openJsonFile("/map/stateCityPath.json");

    std::array<int, 2> hexSize = {mapTextureToDisplay.at(0).getWidth(), mapTextureToDisplay.at(0).getHeight()};

    for (unsigned k = 0; k < (*mapShared)(x, y)->getElements().size(); k++)
    {
        std::shared_ptr<variantElement> variant = (*mapShared)(x, y)->getElements()[k];

        int index;
        std::string path;

        std::visit([&index](auto &&arg)
                   { index = (int)arg.getType(); },
                   *variant);

        if (std::holds_alternative<shared::City>(*variant))
        {
            shared::City city = std::get<shared::City>(*variant);

            if (city.isStateCity)
            {
                path = stateCityData[(int)city.stateCityType]["path"].asString();
            }
            else
            {
                path = elementData[index]["path"].asString();

                path = path.substr(0, 20) + std::to_string(getPlayerNumber(city.player)) + (city.isMature ? "-mature" : (city.isCapital ? "-capital" : "-city")) + ".png";
            }
        }
        else if (std::holds_alternative<shared::ControlPawn>(*variant))
        {
            shared::ControlPawn pawn = std::get<shared::ControlPawn>(*variant);

            path = elementData[index]["path"].asString();

            path = path.substr(0, 20) + std::to_string(getPlayerNumber(pawn.player)) + (pawn.isReinforced() ? "-reinforced" : "") + ".png";

        }
        else if (std::holds_alternative<shared::Caravan>(*variant))
        {
            shared::Caravan caravan = std::get<shared::Caravan>(*variant);

            path = elementData[index]["path"].asString();

            path = path.substr(0, 20) + std::to_string(getPlayerNumber(caravan.player)) + path.substr(21);
        }
        else
        {
            path = elementData[index]["path"].asString();
        }

        path = RESOURCES_PATH + path;

        elementTextureToDisplay[path]->addSprite();

        elementTextureToDisplay[path]->setSpritePosition(elementTextureToDisplay[path]->getSize() - 1, x, y, firstHexagonPosition[0], firstHexagonPosition[1], hexSize);
    }

    if ((*mapShared)(x, y)->hexResource != nullptr)
    {
        shared::Resource resource = *(*mapShared)(x, y)->hexResource;

        int index = (int)resource.getType();

        std::string path = RESOURCES_PATH + resourceData[index]["path"].asString();

        elementTextureToDisplay[path]->addSprite();

        elementTextureToDisplay[path]->setSpritePosition(elementTextureToDisplay[path]->getSize() - 1, x, y, firstHexagonPosition[0], firstHexagonPosition[1], hexSize);
    }
}

int GameWindow::getPlayerNumber(std::string username)
{
    for (unsigned i = 0; i < whoIsPlayingButtons.size(); i++)
    {
        std::string buttonText = whoIsPlayingButtons[i].buttonText->getString();
        if (username.compare(buttonText) == 0)
        {
            return i + 1;
        }
    }
    return 1;
}

/*!
 * @brief Get position of number of boxes and boxes on priority cards
 * @param boxXProportion proportion of the box on x axis
 * @param boxYProportion proportion of the box on y axis
 * @param priorityCard pointer to the card you want to setUp the text
 */
sf::Vector2i GameWindow::getBoxesElementsPosition(float boxXProportion, float boxYProportion, GraphicCard &priorityCard)
{
    int xBoxPos;
    int yBoxPos;
    int xBoxOffset;
    int yBoxOffset;

    xBoxOffset = boxXProportion * WINDOW_LENGTH;
    yBoxOffset = boxYProportion * WINDOW_WIDTH;

    xBoxPos = priorityCard.texture->getSprite().getPosition().x + xBoxOffset;
    yBoxPos = priorityCard.texture->getSprite().getPosition().y + yBoxOffset;

    return (sf::Vector2i(xBoxPos, yBoxPos));
}

/*!
 * @brief Load all the HUD textures
 */
void GameWindow::loadHudTexture()
{

    int rotation = 0;
    int priorityCardIndex = 0;

    const Json::Value &dataNumber = openJsonFile("/hud/data-number.json");

    backgroundTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/hud/background.png");
    backgroundTexture->addSprite();
    float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(WINDOW_LENGTH));
    backgroundTexture->setHudSpritePosition(backgroundScale, WINDOW_LENGTH, WINDOW_WIDTH, rotation, priorityCardIndex);

    const Json::Value &data = openJsonFile("/hud/files.json");

    for (unsigned index = 0; index < data.size(); ++index)
    {
        hudTextureToDisplay.emplace_back(RESOURCES_PATH + data[index]["path"].asString());
        hudTextureToDisplay.back().addSprite();
        float scale = data[index]["scale"].asFloat() / (float(hudTextureToDisplay.back().getWidth()) / float(WINDOW_LENGTH));
        hudTextureToDisplay.back().setImageType((HudTextureType)index);
        hudTextureToDisplay.back().setHudSpritePosition(scale, WINDOW_LENGTH, WINDOW_WIDTH, data[index]["rotation"].asInt(), priorityCardIndex);
    }

    // load the priorityCard
    boxTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/hud/box.png");
    std::vector<int> numberOfBoxesPerCard = {2, 4, 2, 1, 0}; // sent by the server
    std::string boxString = "0";

    if (!titleFont.loadFromFile(RESOURCES_PATH "/font/EnchantedLand.otf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }

    if (!bodyFont.loadFromFile(RESOURCES_PATH "/font/MorrisRomanBlack.otf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }

    const Json::Value &priorityData = openJsonFile("/hud/priority-card.json");

    float priorityTitleTextProportion = dataNumber["priority-card-title-proportion"].asFloat();
    float priorityBodyTextProportion = dataNumber["priority-card-body-proportion"].asFloat();

    for (unsigned index = 0; index < priorityData.size(); ++index)
    {
        priorityCards.emplace_back(
            RESOURCES_PATH + priorityData[index]["path"].asString(),
            dataNumber,
            WINDOW_LENGTH,
            WINDOW_WIDTH,
            index,
            bodyFont);

        // title and body

        setUpText(
            priorityCards.back(),
            priorityData[index]["title"].asString(),
            priorityData[index]["body"][priorityCards.back().level].asString(),
            titleFont,
            bodyFont,
            dataNumber,
            priorityTitleTextProportion,
            priorityBodyTextProportion);

        // boxes

        boxString = std::to_string(numberOfBoxesPerCard[index]) + " x";

        priorityCards.back().nbOfBoxesText = std::make_unique<sf::Text>(
            boxString,
            titleFont,
            dataNumber["box-number-text-size"].asInt());

        sf::Vector2i boxNumberPosition = getBoxesElementsPosition(
            dataNumber["box-x-number-offset-proportion"].asFloat(),
            dataNumber["box-y-number-offset-proportion"].asFloat(),
            priorityCards.back());

        priorityCards.back().nbOfBoxesText->setPosition(boxNumberPosition.x, boxNumberPosition.y);

        boxTexture->addSprite();
        sf::Vector2i boxPosition = getBoxesElementsPosition(
            dataNumber["box-x-offset-proportion"].asFloat(),
            dataNumber["box-y-offset-proportion"].asFloat(),
            priorityCards.back());
        boxTexture->getSprite(index).setPosition(boxPosition.x, boxPosition.y);
    }

    // actionCard
    const Json::Value &actionCardData = openJsonFile("/hud/action-card.json");
    float actionTitleTextProportion = dataNumber["action-card-title-proportion"].asFloat();
    float actionBodyTextProportion = dataNumber["action-card-body-proportion"].asFloat();

    std::vector<int> actionCardOwned = {1, 3, 7}; // array that will be sent by shared

    for (unsigned index = 0; index < actionCardOwned.size(); ++index)
    {
        actionCardsToDisplay.emplace_back(
            RESOURCES_PATH + actionCardData[actionCardOwned[index]]["path"].asString(),
            ACTION_CARD_PROPORTION,
            WINDOW_LENGTH,
            WINDOW_WIDTH,
            actionCardOwned[index],
            index);

        std::string titleCardAction = actionCardData[actionCardOwned[index]]["type"].asString();
        std::string bodyCardAction = actionCardData[actionCardOwned[index]]["body"].asString();

        setUpText(
            actionCardsToDisplay.back(),
            titleCardAction,
            bodyCardAction,
            titleFont,
            bodyFont,
            dataNumber,
            actionTitleTextProportion,
            actionBodyTextProportion);
    }

    // rotation of the techWheel
    const Json::Value &dataRotation = openJsonFile("/hud/tech-wheel-rotation.json");
    for (unsigned index = 0; index < dataRotation.size(); ++index)
    {
        techWheelRotation[index] = dataRotation[index]["rotation"].asInt();
    }

    // text for the user
    std::string textForTheUserString = "";
    textForTheUser = std::make_shared<Button>(sf::Vector2f(dataNumber["text-for-user-size-x"].asFloat(), dataNumber["text-for-user-size-y"].asFloat()),
                                              sf::Vector2f(dataNumber["text-for-user-pos-x"].asFloat(), dataNumber["text-for-user-pos-y"].asFloat()),
                                              TEXT_FOR_USER_BUTTON_COLOR,
                                              false);
    textForTheUser->setText(dataNumber["text-for-user-string-size"].asInt(),
                            sf::Vector2f(0, 0),
                            textForTheUserString,
                            bodyFont,
                            dataNumber["text-for-user-max-char"].asInt());
    textForTheUser->buttonText->setFillColor(TEXT_FOR_USER_COLOR);

    // end of round button
    endOfRoundButton = std::make_unique<Button>(sf::Vector2f(dataNumber["end-of-round-size-x"].asFloat(), dataNumber["end-of-round-size-y"].asFloat()),
                                                sf::Vector2f(dataNumber["text-for-user-pos-x"].asFloat(), dataNumber["end-of-round-pos-y"].asFloat()),
                                                END_OF_TURN_BUTTON_COLOR,
                                                false);
    endOfRoundButton->setText(dataNumber["end-of-round-text-size"].asInt(),
                              sf::Vector2f(0, 0),
                              "End of turn",
                              bodyFont);
    endOfRoundButton->buttonRect->setOutlineThickness(2.0f);
}

/*!
 * @brief Function to modify the text that is specified at a player
 * @param text string: text we want to set
 */
void GameWindow::modifyTextForUser(std::string text)
{
    textForTheUser->buttonText->setString(text);
    textForTheUser->centerText();
}

void GameWindow::addPlayer(std::string username)
{
    for (auto &button : whoIsPlayingButtons)
    {
        std::string buttonText = button.buttonText->getString();
        if (!username.compare(buttonText))
        {
            return;
        }
    }

    whoIsPlayingButtons.emplace_back(
        sf::Vector2f(75, 90 / 2),
        sf::Vector2f(0, 0),
        PLAYER_COLOR[whoIsPlayingButtons.size()],
        false);

    whoIsPlayingButtons.back().setText(18, sf::Vector2f(0, 0), username, titleFont);

    for (unsigned i = 0; i < whoIsPlayingButtons.size(); i++)
    {
        whoIsPlayingButtons[i].buttonRect->setPosition(
            (WINDOW_LENGTH - 75 * whoIsPlayingButtons.size() - 30 * (whoIsPlayingButtons.size() - 1)) / 2 + 105 * i,
            0);
        whoIsPlayingButtons[i].centerText(true);
    }
}

/*!
 * @brief Function that deteck where the user click and what to send to the engine
 * @param timeSecond is a boolean used to
 */
long GameWindow::getCurrentTime(bool timeSecond)
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

/*!
 * @brief Function to set the winnee window at the end of the party
 * @param winner string: who is the winner
 * @param causes string: why the winner have won
 */
void GameWindow::setWinnerWindow(std::string winner, std::string causes)
{

    winnerWindow->title->setString(winner + " is the winner !!!");
    winnerWindow->body->setString(causes);
    winnerWindow->centerText();
    winnerWindow->isWindowActive = true;
}

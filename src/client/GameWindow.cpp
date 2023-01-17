#include <client.hpp>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <cmath>
#include <string>
#include <codecvt>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50
#define MAP_WIDTH 15
#define MAP_HEIGHT 11

#define NUMBER_OF_FIELD 12

#define TCHAT_MAX_SIZE 200

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define ACTION_CARD_PROPORTION 0.125
#define TITLE_PROPORTION 0.025
#define BODY_PROPORTION_X 0.0075
#define BODY_PROPORTION_Y 0.055
#define MAX_CHARACTER_SIZE 19
#define NBR_CHAR_MAX_PER_LIGNE 22
#define TURN_NUMBER 2

#define ASCI_BEGIN 19
#define ASCI_END 127

#define CARD_BORDER 18

#define ELEMENT_PATH "/img/map/element/"

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

const std::vector<sf::Color> PLAYER_COLOR = {sf::Color(119, 238, 217, 160), sf::Color(251, 76, 255, 160), sf::Color(93, 109, 126, 160), sf::Color(230, 176, 170, 160)};
const sf::Color TEXT_COLOR = sf::Color(240, 230, 230);

using namespace client;

/*!
 * @brief Constructor
 *
 * Constructor of GameWindow class
 */
GameWindow::GameWindow()
{
    firstHexagonPosition = {MAP_X_OFFSET, MAP_Y_OFFSET};

    loadMapTexture();
    loadElementTexture();
    updateElementTexture();
    loadHudTexture();
    chatBox = std::make_unique<Chat>(bodyFont);

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

    for (auto &elementTexture : elementTextureToDisplay)
    {
        elementTexture.second->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);
    }

    for (auto &priorityCardTexture : priorityCards)
    {
        priorityCardTexture.texture->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);
        gameEnginePtr->clientWindow->draw(*priorityCardTexture.title);
        gameEnginePtr->clientWindow->draw(*priorityCardTexture.nbOfBoxesText);
        if (priorityCardTexture.isUp)
        {
            gameEnginePtr->clientWindow->draw(*priorityCardTexture.body);
        }
    }

    boxTexture->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);

    for (unsigned i = 0; i < actionCardsToDisplay.size(); i++)
    {
        gameEnginePtr->clientWindow->draw(actionCardsToDisplay[i].texture->getSprite(0));
        gameEnginePtr->clientWindow->draw(*actionCardsToDisplay[i].title);
        gameEnginePtr->clientWindow->draw(*actionCardsToDisplay[i].body);
    }

    for (unsigned i = 0; i < whoIsPlayingButtons.size(); i++)
    {
        gameEnginePtr->clientWindow->draw(*whoIsPlayingButtons[i].buttonRect);
        gameEnginePtr->clientWindow->draw(*whoIsPlayingButtons[i].buttonText);
    }

    gameEnginePtr->clientWindow->draw(hudTextureToDisplay.at(TURN_NUMBER % 5).getSprite());

    for (unsigned i = 5; i < hudTextureToDisplay.size(); i++)
    {
        hudTextureToDisplay[i].drawTextureDisplayerSprite(gameEnginePtr->clientWindow);
    }

    if (isChatOpen)
    {
        chatBox->drawChat(gameEnginePtr->clientWindow); 
    }

    chatPicture->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);
    movePicture->drawTextureDisplayerSprite(gameEnginePtr->clientWindow);

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
        clickAction(event, clickStartingPoint, moveMode);
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
    case sf::Keyboard::Escape:
        gameEnginePtr->handleQuitMenu(true);
        return true;

    case sf::Keyboard::Enter:
        //sendMessage(chatBox->message)
        //line to be delete
        chatBox->updateChat("00:00", "Username", chatBox->message);
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
 * @brief Change the cursor type to a hand or an arrow
 * @param moveMode pointer to know if the map is moving on the screen
 */
void GameWindow::changeMouseCursor(std::shared_ptr<bool> moveMode)
{
    if (*moveMode)
    {
        *moveMode = false;
        if (clientCursor.loadFromSystem(sf::Cursor::Arrow))
        {
            gameEnginePtr->clientWindow->setMouseCursor(clientCursor);
        }
    }
    else
    {
        *moveMode = true;
        if (clientCursor.loadFromSystem(sf::Cursor::Hand))
        {
            gameEnginePtr->clientWindow->setMouseCursor(clientCursor);
        }
    }
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
const auto GameWindow::openJsonFile(std::string path)
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
 * @brief Function that deteck where the user click and what to send to the engine
 * @param clickPosition is the position on the cursor when the user click
 * @brief Dectect click and actions to do after
 */
void GameWindow::clickAction(sf::Event& event, sf::Vector2i clickPosition, std::shared_ptr<bool> moveMode)
{
    int minimumDistance = WINDOW_LENGTH;
    std::array<int, 2> hexagonOnClick = {0, 0};

    bool isClickable = false;

    if (!*moveMode)
    {
        for (unsigned i = 0; i < priorityCards.size(); i++)
        {
            // Check if the click position is inside the sprite
            if (gameEnginePtr->intersectPointRect(clickPosition, priorityCards[i].texture->getSprite().getGlobalBounds()))
            {
                gameEnginePtr->handleInformation(-1, i + 1); // -1 to signify that the space clicked is a priority card
                priorityCards[i].moveUpPriorityCard();
                return;
            }
        }

        for (auto &mapTexture : mapTextureToDisplay)
        {
            for (unsigned j = 0; j < mapTexture.getSize(); j++)
            {

                if (gameEnginePtr->intersectPointRect(clickPosition, mapTexture.getSprite(j).getGlobalBounds()))
                {
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
        }
    }

    // Check if the click position is inside the move map button
    if (gameEnginePtr->intersectPointRect(clickPosition, movePicture->getSprite().getGlobalBounds()))
    {
        if(event.mouseButton.button == sf::Mouse::Left)
        {
            changeMouseCursor(moveMode);
            return;
        }
        changeMouseCursor(moveMode);
        sf::Vector2i nullPosition(0, 0);
        moveMap(nullPosition, {MAP_X_OFFSET, MAP_Y_OFFSET}, true);
        return;
    }

    // Check if the click position is inside the chat button
    if (gameEnginePtr->intersectPointRect(clickPosition, chatPicture->getSprite().getGlobalBounds()))
    {
        isChatOpen = !isChatOpen;
        return;
    }

    if (isClickable)
    {
        gameEnginePtr->handleInformation(hexagonOnClick[0], hexagonOnClick[1]);
    }
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
    float titleTextSizeProportion,
    float bodyTextSizeProportion)
{
    int titleTextSize = titleTextSizeProportion * WINDOW_LENGTH;
    int bodyTextSize = bodyTextSizeProportion * WINDOW_LENGTH;

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
    card.body->setLineSpacing(0.9f);
    int xBodyOffset = BODY_PROPORTION_X * WINDOW_LENGTH;
    int yBodyOffset = BODY_PROPORTION_Y * WINDOW_WIDTH;
    int xBodyPosition = card.texture->getSprite().getPosition().x + xBodyOffset;
    int yBodyPosition = card.texture->getSprite().getPosition().y + yBodyOffset;
    card.body->setPosition(xBodyPosition, yBodyPosition);
}

/*!
 * @brief Load all the textures of the map
 */
void GameWindow::loadMapTexture()
{

    mapShared.generateRandomMap(123456789);

    std::string hexagonImgPath = RESOURCES_PATH "/img/map/field/field-";
    std::array<std::string, 12> mapField = {"water", "grassland", "hill", "forest", "desert", "mountain",
                                            "wonder-everest", "wonder-galapagos", "wonder-kilimanjaro",
                                            "wonder-messa", "wonder-pantanal", "wonder-volcanic"};

    for (unsigned i{0}; i < mapField.size(); i++)
    {
        std::string mapElementPath = hexagonImgPath + mapField.at(i) + ".png";
        mapTextureToDisplay.emplace_back(mapElementPath);
    }

    for (unsigned i = 0; i < mapShared.getMapHeight(); i++)
    {
        for (unsigned j = 0; j < mapShared.getMapWidth(); j++)
        {
            int indexSprite = mapTextureToDisplay.at((int)mapShared(j, i)->getFieldLevel()).getSize();
            mapTextureToDisplay.at((int)mapShared(j, i)->getFieldLevel()).addSprite();
            mapTextureToDisplay.at((int)mapShared(j, i)->getFieldLevel())
                .setSpritePosition(indexSprite, j, i, MAP_X_OFFSET, MAP_Y_OFFSET, {0, 0});
        }
    }
}

/*!
 * @brief Load all the textures of the elements
 */
void GameWindow::loadElementTexture()
{
    std::string folder_path = RESOURCES_PATH "/img/map/element/";
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
        elementTextureToDisplay[path] = (std::unique_ptr<client::TextureDisplayer>)new TextureDisplayer(path);
    }
}

/*!
 * @brief Update all the textures of the map
 */
void GameWindow::updateElementTexture()
{

    for (auto &kv : elementTextureToDisplay)
    {
        kv.second->clearSprites();
    }

    std::array<int, 2> hexSize = {mapTextureToDisplay.at(0).getWidth(), mapTextureToDisplay.at(0).getHeight()};

    // Data are temporary loaded with the json file but it will be updated from the server soon
    const Json::Value &data = openJsonFile("/img/map/files.json");

    for (unsigned index = 0; index < data.size(); ++index)
    {

        std::string path = RESOURCES_PATH + data[index]["path"].asString();

        elementTextureToDisplay[path]->addSprite();

        elementTextureToDisplay[path]->setSpritePosition(elementTextureToDisplay[path]->getSize() - 1, data[index]["y"].asInt(), data[index]["x"].asInt(), firstHexagonPosition[0], firstHexagonPosition[1], hexSize);
    }
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

    const Json::Value &dataNumber = openJsonFile("/img/hud/data-number.json");

    backgroundTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/img/hud/background.png");
    backgroundTexture->addSprite();
    float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(WINDOW_LENGTH));
    backgroundTexture->setHudSpritePosition(backgroundScale, WINDOW_LENGTH, WINDOW_WIDTH, rotation, priorityCardIndex);

    const Json::Value &data = openJsonFile("/img/hud/files.json");

    for (unsigned index = 0; index < data.size(); ++index)
    {
        hudTextureToDisplay.emplace_back(RESOURCES_PATH + data[index]["path"].asString());
        hudTextureToDisplay.back().addSprite();
        float scale = data[index]["scale"].asFloat() / (float(hudTextureToDisplay.back().getWidth()) / float(WINDOW_LENGTH));
        hudTextureToDisplay.back().setImageType((HudTextureType)index);
        hudTextureToDisplay.back().setHudSpritePosition(scale, WINDOW_LENGTH, WINDOW_WIDTH, data[index]["rotation"].asInt(), priorityCardIndex);
    }

    // load the priorityCard
    boxTexture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/img/hud/box.png");
    std::vector<int> numberOfBoxesPerCard = {2, 4, 2, 1, 0}; // sent by the server
    std::string boxString = "0";

    if (!titleFont.loadFromFile(RESOURCES_PATH "/img/hud/font.otf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }

    if (!bodyFont.loadFromFile(RESOURCES_PATH "/img/hud/Calibri.ttf"))
    {
        std::cerr << "Font not loaded" << std::endl;
    }

    const Json::Value &priorityData = openJsonFile("/img/hud/priority-card.json");
    float priorityTitleTextProportion = dataNumber["priority-card-title-proportion"].asFloat();
    float priorityBodyTextProportion = dataNumber["priority-card-body-proportion"].asFloat();

    for (unsigned index = 0; index < priorityData.size(); ++index)
    {
        priorityCards.emplace_back(
            RESOURCES_PATH + priorityData[index]["path"].asString(),
            dataNumber,
            WINDOW_LENGTH,
            WINDOW_WIDTH,
            index);

        // title and body

        setUpText(
            priorityCards.back(),
            priorityData[index]["title"].asString(),
            priorityData[index]["body"][priorityCards.back().level].asString(),
            titleFont,
            bodyFont,
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

        chatPicture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/img/hud/chat-button.png");
        chatPicture->addSprite();
        chatPicture->getSprite(0).setPosition(sf::Vector2f(20, 585));

        movePicture = std::make_unique<TextureDisplayer>(RESOURCES_PATH "/img/hud/move-button.png");
        movePicture->addSprite();
        movePicture->getSprite(0).setPosition(sf::Vector2f(20, 653));
    }

    // actionCard
    const Json::Value &actionCardData = openJsonFile("/img/hud/action-card.json");
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
            actionTitleTextProportion,
            actionBodyTextProportion);
    }

    // isPlaying buttons
    int whoIsPlaying = 2; // sent by the server (temporary)

    for (int i = 0; i < dataNumber["nb-player"].asInt(); i++)
    {
        bool isPlaying;
        (i + 1 == whoIsPlaying) ? isPlaying = true : isPlaying = false;
        std::string text = "Player ";
        text += std::to_string(i + 1);
        int offset = dataNumber["offset-between-up-player"].asInt();
        int upPosition = (WINDOW_LENGTH + (float(2 / 3) - dataNumber["nb-player"].asInt()) * offset) / 2;

        whoIsPlayingButtons.emplace_back(
            sf::Vector2f(offset * float(float(2) / float(3)), offset / 2),
            sf::Vector2f(upPosition + offset * i, 0),
            PLAYER_COLOR[i],
            isPlaying);

        whoIsPlayingButtons.back().setText(dataNumber["up-player-text-size"].asInt(), sf::Vector2f(0, 0), text, titleFont);
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

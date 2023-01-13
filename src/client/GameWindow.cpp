#include <client.hpp>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <cmath>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50
#define MAP_WIDTH 15
#define MAP_HEIGHT 11

#define NUMBER_OF_FIELD 12

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define ACTION_CARD_PROPORTION 0.125
#define TITLE_PROPORTION 0.025
#define BODY_PROPORTION_X 0.0075
#define BODY_PROPORTION_Y 0.055
#define MAX_CHARACTER_SIZE 19
#define NBR_CHAR_MAX_PER_LIGNE 22
#define TURN_NUMBER 2

#define CARD_BORDER 18

#define ELEMENT_PATH "/img/map/element/"

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "../resources"
#endif

const std::vector<sf::Color> PLAYER_COLOR = {sf::Color(119, 238, 217, 160), sf::Color(251, 76, 255, 160), sf::Color(93, 109, 126, 160), sf::Color(230, 176, 170, 160)};
const sf::Color TEXT_COLOR = sf::Color(240, 230, 230);

namespace client
{
    /*!
     * \brief Constructor
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
    }

    /*!
     * \brief Display all the different variable in the screen
     */
    void GameWindow::displayWindow()
    {

        clientGameWindow->clear(sf::Color::Blue);

        clientGameWindow->draw(backgroundTexture->getSprite());

        for (unsigned i = 0; i < mapTextureToDisplay.size(); i++)
        {
            for (unsigned j = 0; j < mapTextureToDisplay[i].getSize(); j++)
            {
                clientGameWindow->draw(mapTextureToDisplay[i].getSprite(j));
            }
        }

        for (auto &kv : elementTextureToDisplay)
        {
            kv.second->drawElementSprite(clientGameWindow);
        }

        for (unsigned i = 0; i < priorityCards.size(); i++)
        {
            clientGameWindow->draw(priorityCards[i].texture->getSprite(0));
            clientGameWindow->draw(*priorityCards[i].title);
            clientGameWindow->draw(boxTexture->getSprite(i));
            clientGameWindow->draw(*priorityCards[i].nbOfBoxesText);
            if (priorityCards[i].isUp)
            {
                clientGameWindow->draw(*priorityCards[i].body);
            }
        }

        for (unsigned i = 0; i < actionCardsToDisplay.size(); i++)
        {
            clientGameWindow->draw(actionCardsToDisplay[i].texture->getSprite(0));
            clientGameWindow->draw(*actionCardsToDisplay[i].title);
            clientGameWindow->draw(*actionCardsToDisplay[i].body);
        }

        for (unsigned i = 0; i < whoIsPlayingButtons.size(); i++)
        {
            clientGameWindow->draw(*whoIsPlayingButtons[i].buttonRect);
            clientGameWindow->draw(*whoIsPlayingButtons[i].buttonText);
        }

        clientGameWindow->draw(hudTextureToDisplay.at(TURN_NUMBER % 5).getSprite());

        for (unsigned i = 5; i < hudTextureToDisplay.size(); i++)
        {
            for (unsigned j = 0; j < hudTextureToDisplay[i].getSize(); j++)
            {
                clientGameWindow->draw(hudTextureToDisplay[i].getSprite(j));
            }
        }

        clientGameWindow->display();
    }

    /*!
     * \brief Loop that look for events to happend and call displayWindow()
     */
    void GameWindow::startGame(std::shared_ptr<sf::RenderWindow> clientWindow, std::function<void(bool)> quitGame, std::function<void(int, int)> callback)
    {
        quitGameWindow = quitGame;
        clickEvent = callback;
        clientGameWindow = clientWindow;

        std::shared_ptr<bool> moveMode = std::make_unique<bool>(false);
        std::shared_ptr<bool> clickMode = std::make_unique<bool>(false);

        std::array<int, 2> newMapOffset;

        sf::Vector2i clickStartingPoint;

        long lastUpdateTimer = getCurrentTime(false);

        while (clientGameWindow->isOpen())
        {

            if (getCurrentTime(false) - lastUpdateTimer > (100 / 3))
            {
                displayWindow();
                lastUpdateTimer = getCurrentTime(false);
            }

            // handle events
            sf::Event event;
            while (clientGameWindow->pollEvent(event))
            {
                if (gameEventHappened(&event, &clickStartingPoint, &newMapOffset, moveMode, clickMode)) return;
            }
        }
    }

    bool GameWindow::gameEventHappened(sf::Event* event, sf::Vector2i* clickStartingPoint, std::array<int, 2>* newMapOffset, std::shared_ptr<bool> moveMode, std::shared_ptr<bool> clickMode)
    {
        switch (event->type)
        {
        case sf::Event::MouseButtonPressed:

            *clickMode = true;

            *clickStartingPoint = sf::Mouse::getPosition(*clientGameWindow);

            if (!*moveMode)
                clickAction(*clickStartingPoint);

            break;

        case sf::Event::MouseButtonReleased:

            *clickMode = false;

            break;

        case sf::Event::MouseMoved:

            if (*moveMode && *clickMode)
            {

                *newMapOffset = {sf::Mouse::getPosition(*clientGameWindow).x - clickStartingPoint->x,
                                 sf::Mouse::getPosition(*clientGameWindow).y - clickStartingPoint->y};

                firstHexagonPosition = {firstHexagonPosition[0] + newMapOffset->at(0),
                                        firstHexagonPosition[1] + newMapOffset->at(1)};

                for (unsigned i = 0; i < mapTextureToDisplay.size(); i++)
                    mapTextureToDisplay[i].moveSpritePosition(newMapOffset->at(0), newMapOffset->at(1));

                for (auto &kv : elementTextureToDisplay)
                {
                    kv.second->moveSpritePosition(newMapOffset->at(0), newMapOffset->at(1));
                }

                *clickStartingPoint = sf::Mouse::getPosition(*clientGameWindow);
            }
            break;

        case sf::Event::KeyPressed:

            switch (event->key.code)
            {
            case sf::Keyboard::M:
                if (*moveMode)
                {
                    *moveMode = false;
                    if (clientCursor.loadFromSystem(sf::Cursor::Arrow))
                        clientGameWindow->setMouseCursor(clientCursor);
                }
                else
                {
                    *moveMode = true;
                    if (clientCursor.loadFromSystem(sf::Cursor::Hand))
                        clientGameWindow->setMouseCursor(clientCursor);
                }
                break;

            case sf::Keyboard::K:
                if (clientCursor.loadFromSystem(sf::Cursor::Arrow))
                    clientGameWindow->setMouseCursor(clientCursor);
                quitGameWindow(false);
                return true;

            case sf::Keyboard::Escape:
                quitGameWindow(true);
                return true;

            case sf::Keyboard::L:

                *newMapOffset = {MAP_X_OFFSET - firstHexagonPosition[0],
                                 MAP_Y_OFFSET - firstHexagonPosition[1]};

                firstHexagonPosition = {MAP_X_OFFSET, MAP_Y_OFFSET};

                for (unsigned i = 0; i < mapTextureToDisplay.size(); i++)
                {
                    mapTextureToDisplay[i].moveSpritePosition(newMapOffset->at(0), newMapOffset->at(1));
                }

                for (auto &kv : elementTextureToDisplay)
                {
                    kv.second->moveSpritePosition(newMapOffset->at(0), newMapOffset->at(1));
                }

                *clickStartingPoint = sf::Mouse::getPosition(*clientGameWindow);

                break;

            default:
                break;
            }
            break;

        case sf::Event::Closed:
            quitGameWindow(true);
            return true;

        default:
            break;
        }
        return false;
    }

    /*!
     * \brief Open JSON File
     */
    const auto GameWindow::openJsonFile(std::string path)
    {

        std::ifstream file(RESOURCES_PATH + path);
        // check is file is correctly open
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
     * \brief Move up priority cards when we click on it
     */

    void GameWindow::moveUpPriorityCard(CardStruct *card)
    {

        const Json::Value &dataNumber = openJsonFile("/img/hud/data-number.json");

        float priorityScale = dataNumber["priority-card-proportion"].asFloat() / (float(card->texture->getWidth()) / float(WINDOW_LENGTH));

        int yPos;
        int xPos;
        int xTitlePos;
        int xBodyPosition;

        if (!card->isUp)
        {
            yPos = WINDOW_WIDTH - card->texture->getHeight() * priorityScale;
            card->isUp = true;
        }
        else
        {
            yPos = WINDOW_WIDTH - card->texture->getHeight() * priorityScale + dataNumber["priority-card-up-scale"].asFloat() * WINDOW_WIDTH;
            card->isUp = false;
        }

        xPos = card->texture->getSprite().getPosition().x;
        xTitlePos = card->title->getPosition().x;
        xBodyPosition = card->body->getPosition().x;

        int yBodyOffset = BODY_PROPORTION_Y * WINDOW_WIDTH;

        card->texture->getSprite().setPosition(xPos, yPos);
        card->title->setPosition(xTitlePos, yPos);
        card->body->setPosition(xBodyPosition, yPos + yBodyOffset);
    }

    /*!
     * \brief Dectect click and actions to do after
     */
    void GameWindow::clickAction(sf::Vector2i clickPosition)
    {
        int minimumDistance = WINDOW_LENGTH;
        std::array<int, 2> hexagonOnClick = {0, 0};

        sf::FloatRect cursorRect = mapTextureToDisplay[0].getSprite(0).getGlobalBounds();
        cursorRect.left = clickPosition.x;
        cursorRect.top = clickPosition.y;
        cursorRect.width = 1;
        cursorRect.height = 1;

        bool isClickable = false;

        for (unsigned i = 0; i < priorityCards.size(); i++)
        {
            sf::FloatRect spriteBounds = priorityCards[i].texture->getSprite().getGlobalBounds();

            if (spriteBounds.intersects(cursorRect))
            {
                clickEvent(-1, i + 1); // -1 to signify that the space clicked is a priority card
                moveUpPriorityCard(&priorityCards[i]);
                return;
            }
        }

        for (auto &mapTexture : mapTextureToDisplay)
        {

            for (unsigned j = 0; j < mapTexture.getSize(); j++)
            {

                sf::FloatRect spriteBounds = mapTexture.getSprite(j).getGlobalBounds();

                if (spriteBounds.intersects(cursorRect))
                {

                    isClickable = true;

                    int distance = sqrt(pow(spriteBounds.left + spriteBounds.width / 2 - cursorRect.left, 2) + pow(spriteBounds.top + spriteBounds.height / 2 - cursorRect.top, 2));

                    if (distance < minimumDistance)
                    {

                        minimumDistance = distance;
                        hexagonOnClick[1] = (int)((spriteBounds.top - firstHexagonPosition[1])) / (int)((spriteBounds.height * 3 / 4));
                        hexagonOnClick[0] = (int)((spriteBounds.left - firstHexagonPosition[0])) / (int)((spriteBounds.width - 1));
                    }
                }
            }
        }

        if (isClickable)
        {
            clickEvent(hexagonOnClick[0], hexagonOnClick[1]);
        }
    }

    /*!
     * \brief Display text on the cards
     */
    void GameWindow::setUpText(std::vector<CardStruct> *cards, std::string title, std::string body, sf::Font *titleFont, sf::Font *bodyFont, float titleTextSizeProportion, float bodyTextSizeProportion)
    {
        int titleTextSize = titleTextSizeProportion * WINDOW_LENGTH;
        int bodyTextSize = bodyTextSizeProportion * WINDOW_LENGTH;

        // display the title on the card
        cards->back().title = (std::unique_ptr<sf::Text>)new sf::Text(title, *titleFont, titleTextSize);
        cards->back().title->setStyle(sf::Text::Bold);
        cards->back().title->setFillColor(TEXT_COLOR);
        auto titleSize = cards->back().title->getLocalBounds();
        int xTitleOffset = (cards->back().texture->getWidth() - titleSize.width) / 2;
        int xTitlePosition = cards->back().texture->getSprite().getPosition().x + xTitleOffset;
        int yTitlePosition = cards->back().texture->getSprite().getPosition().y;
        cards->back().title->setPosition(xTitlePosition, yTitlePosition);

        // display the body on the card
        cards->back().body = (std::unique_ptr<sf::Text>)new sf::Text(body, *bodyFont, bodyTextSize);

        // to have the text on several lines without exceeding the card
        int countEndLine = 1;
        while (cards->back().body->getLocalBounds().width > cards->back().texture->getWidth() - CARD_BORDER) // 18 to not touch the black border
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
            cards->back().body->setString(body);
        }

        cards->back().body->setFillColor(TEXT_COLOR);
        cards->back().body->setLineSpacing(0.9f);
        int xBodyOffset = BODY_PROPORTION_X * WINDOW_LENGTH;
        int yBodyOffset = BODY_PROPORTION_Y * WINDOW_WIDTH;
        int xBodyPosition = cards->back().texture->getSprite().getPosition().x + xBodyOffset;
        int yBodyPosition = cards->back().texture->getSprite().getPosition().y + yBodyOffset;
        cards->back().body->setPosition(xBodyPosition, yBodyPosition);
    }

    /*!
     * \brief Load all the textures of the map
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
                mapTextureToDisplay.at((int)mapShared(j, i)->getFieldLevel()).setSpritePosition(indexSprite, j, i, MAP_X_OFFSET, MAP_Y_OFFSET, {0, 0});
            }
        }
    }

    /*!
     * \brief Load all the textures of the map
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
                continue;

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
     * \brief Load all the textures of the map
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
     * \brief Get position of number of boxes and boxes on priority cards
     */

    sf::Vector2i GameWindow::getBoxesElementsPosition(float boxXProportion, float boxYProportion, CardStruct *priorityCard)
    {
        int xBoxPos;
        int yBoxPos;
        int xBoxOffset;
        int yBoxOffset;

        xBoxOffset = boxXProportion * WINDOW_LENGTH;
        yBoxOffset = boxYProportion * WINDOW_WIDTH;

        xBoxPos = priorityCard->texture->getSprite().getPosition().x + xBoxOffset;
        yBoxPos = priorityCard->texture->getSprite().getPosition().y + yBoxOffset;

        return (sf::Vector2i(xBoxPos, yBoxPos));
    }

    /*!
     * \brief Load all the HUD textures
     */

    void GameWindow::loadHudTexture()
    {

        int rotation = 0;
        int priorityCardIndex = 0;

        const Json::Value &dataNumber = openJsonFile("/img/hud/data-number.json");

        backgroundTexture = (std::unique_ptr<TextureDisplayer>)new TextureDisplayer(RESOURCES_PATH "/img/hud/background.png");
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
        boxTexture = (std::unique_ptr<TextureDisplayer>)new TextureDisplayer(RESOURCES_PATH "/img/hud/box.png");
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
            priorityCards.emplace_back();
            priorityCards.back().texture = (std::unique_ptr<client::TextureDisplayer>)new TextureDisplayer(RESOURCES_PATH + priorityData[index]["path"].asString());
            priorityCards.back().texture->addSprite();
            float priorityScale = dataNumber["priority-card-proportion"].asFloat() / (float(priorityCards.back().texture->getWidth()) / float(WINDOW_LENGTH));
            priorityCards.back().texture->setImageType((HudTextureType)(index + 7)); // +7 to go to the priority cards in the HudTextureType (enum class)
            priorityCards.back().texture->setHudSpritePosition(priorityScale, WINDOW_LENGTH, WINDOW_WIDTH, 0, index);
            priorityCards.back().level = 0; // sent by the server
            priorityCards.back().isUp = false;

            // title and body

            setUpText(&priorityCards, priorityData[index]["title"].asString(), priorityData[index]["body"][priorityCards.back().level].asString(), &titleFont, &bodyFont, priorityTitleTextProportion, priorityBodyTextProportion);

            // boxes

            boxString = std::to_string(numberOfBoxesPerCard[index]);
            boxString += " x";
            priorityCards.back().nbOfBoxesText = (std::unique_ptr<sf::Text>)new sf::Text(boxString, titleFont, dataNumber["box-number-text-size"].asInt());
            sf::Vector2i boxNumberPosition = getBoxesElementsPosition(dataNumber["box-x-number-offset-proportion"].asFloat(), dataNumber["box-y-number-offset-proportion"].asFloat(), &priorityCards.back());
            priorityCards.back().nbOfBoxesText->setPosition(boxNumberPosition.x, boxNumberPosition.y);

            boxTexture->addSprite();
            sf::Vector2i boxPosition = getBoxesElementsPosition(dataNumber["box-x-offset-proportion"].asFloat(), dataNumber["box-y-offset-proportion"].asFloat(), &priorityCards.back());
            boxTexture->getSprite(index).setPosition(boxPosition.x, boxPosition.y);
        }

        // actionCard

        const Json::Value &actionCardData = openJsonFile("/img/hud/action-card.json");
        float actionTitleTextProportion = dataNumber["action-card-title-proportion"].asFloat();
        float actionBodyTextProportion = dataNumber["action-card-body-proportion"].asFloat();

        std::vector<int> actionCardOwned = {1, 3, 7}; // array that will be sent by shared

        for (unsigned index = 0; index < actionCardOwned.size(); ++index)
        {
            actionCardsToDisplay.emplace_back();
            actionCardsToDisplay.back().texture = (std::unique_ptr<client::TextureDisplayer>)new TextureDisplayer(RESOURCES_PATH + actionCardData[actionCardOwned[index]]["path"].asString());
            actionCardsToDisplay.back().texture->addSprite();
            float actionScale = ACTION_CARD_PROPORTION / (float(actionCardsToDisplay.back().texture->getWidth()) / float(WINDOW_LENGTH));
            actionCardsToDisplay.back().texture->setImageType((HudTextureType)(actionCardOwned[index] + 11)); // +11 to go to the action cards in the HudTextureType (enum class)
            actionCardsToDisplay.back().texture->setHudSpritePosition(actionScale, WINDOW_LENGTH, WINDOW_WIDTH, 0, index);

            std::string titleCardAction = actionCardData[actionCardOwned[index]]["type"].asString();
            std::string bodyCardAction = actionCardData[actionCardOwned[index]]["body"].asString();

            setUpText(&actionCardsToDisplay, titleCardAction, bodyCardAction, &titleFont, &bodyFont, actionTitleTextProportion, actionBodyTextProportion);
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

            whoIsPlayingButtons.emplace_back(sf::Vector2f(offset * float(float(2) / float(3)), offset / 2), sf::Vector2f(upPosition + offset * i, 0), PLAYER_COLOR[i], isPlaying);
            whoIsPlayingButtons.back().setText(dataNumber["up-player-text-size"].asInt(), sf::Vector2f(0, 0), text, &titleFont);
        }
    }

    long GameWindow::getCurrentTime(bool timeSecond)
    {
        if (timeSecond)
            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        else
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

}

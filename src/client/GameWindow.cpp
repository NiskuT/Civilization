#include <client.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <json/json.h>
#include <cmath>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50
#define MAP_WIDTH 15
#define MAP_HEIGHT 11

#define NUMBER_OF_FIELD 12

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define PRIORITY_CARD_PROPORTION 0.144
#define ACTION_CARD_PROPORTION 0.125
#define TITLE_PROPORTION 0.025
#define BODY_PROPORTION_X 0.0075
#define BODY_PROPORTION_Y 0.05
#define MAX_CHARACTER_SIZE 18
#define NBR_CHAR_MAX_PER_LIGNE 22 
#define TURN_NUMBER 2

#ifndef RESOURCES_PATH
    #define RESOURCES_PATH "../resources"
#endif

namespace client
{

/*!
 * \brief Constructor
 *
 * Constructor of GameWindow class
 */
GameWindow::GameWindow() {
    clientGameWindow.create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Civilization VII", sf::Style::Close);
    clientGameWindow.setPosition(sf::Vector2i(0, 0));

    firstHexagonPosition = {MAP_X_OFFSET, MAP_Y_OFFSET};

    loadMapTexture();
    loadElementTexture();
    loadHudTexture();
}

/*!
* \brief Display all the different variable in the screen
*/
void GameWindow::displayWindow() {

    clientGameWindow.clear(sf::Color::Blue);

    clientGameWindow.draw(backgroundTexture->getSprite());

    for(unsigned i = 0; i < mapTextureToDisplay.size(); i++ ){
        for(unsigned j = 0; j < mapTextureToDisplay[i].getSize(); j++ ){
            clientGameWindow.draw(mapTextureToDisplay[i].getSprite(j));
        }
    }

    for(auto& kv : elementTextureToDisplay){
        for(unsigned j = 0; j < kv.second->getSize(); j++ ){
            clientGameWindow.draw(kv.second->getSprite(j));
        }
    }

    for (unsigned i = 0; i < priorityCards.size(); i++){
        clientGameWindow.draw(priorityCards[i].texture->getSprite(0));   
        clientGameWindow.draw(*priorityCards[i].title);
        clientGameWindow.draw(*priorityCards[i].body);
    }

    for (unsigned i = 0; i < actionCardsToDisplay.size(); i++){
        clientGameWindow.draw(actionCardsToDisplay[i].texture->getSprite(0));
        clientGameWindow.draw(*actionCardsToDisplay[i].title);
        clientGameWindow.draw(*actionCardsToDisplay[i].body);


    }


    clientGameWindow.draw(hudTextureToDisplay.at(TURN_NUMBER%5).getSprite());
    
    for(unsigned i = 5; i < hudTextureToDisplay.size(); i++ ){
        for(unsigned j = 0; j < hudTextureToDisplay[i].getSize(); j++ ){            
            clientGameWindow.draw(hudTextureToDisplay[i].getSprite(j));
        }
    }
    clientGameWindow.display();
}

/*!
 * \brief Loop that look for events to happend and call displayWindow()
 */
void GameWindow::clientWindow(std::function<void(int, int)> callback)
{
    int moveMode = false;
    int clickMode = false;

    sf::Vector2i clickStartingPoint;
    std::array<int, 2> newMapOffset;

    long lastUpdateTimer = getCurrentTime(false);

    while (clientGameWindow.isOpen()){

        if (getCurrentTime(false) - lastUpdateTimer > (100/3)){
            mutexGame.lock();
            displayWindow();
            mutexGame.unlock();
            lastUpdateTimer = getCurrentTime(false);
        }
        
        // handle events
        sf::Event event;
        while (clientGameWindow.pollEvent(event))
        {
            
            switch (event.type)
            {
            case sf::Event::MouseButtonPressed:

                clickMode = true;
        
                clickStartingPoint = sf::Mouse::getPosition(clientGameWindow);

                if (!moveMode) clickAction(clickStartingPoint, callback);

                break;

            case sf::Event::MouseButtonReleased:

                clickMode = false;

                break;

            case sf::Event::MouseMoved:

                if (moveMode && clickMode){

                    newMapOffset = {sf::Mouse::getPosition(clientGameWindow).x - clickStartingPoint.x,
                                    sf::Mouse::getPosition(clientGameWindow).y - clickStartingPoint.y};

                    firstHexagonPosition = {firstHexagonPosition[0] + newMapOffset[0], 
                                            firstHexagonPosition[1] + newMapOffset[1]};

                    for(unsigned i = 0; i < mapTextureToDisplay.size(); i++){
                        mutexGame.lock();
                        mapTextureToDisplay[i].moveSpritePosition(newMapOffset[0], newMapOffset[1]);
                        mutexGame.unlock();
                    }

                    for(auto& kv : elementTextureToDisplay){
                        mutexGame.lock();
                        kv.second->moveSpritePosition(newMapOffset[0], newMapOffset[1]);
                        mutexGame.unlock();
                    }

                    clickStartingPoint = sf::Mouse::getPosition(clientGameWindow);
                }
                break;

            case sf::Event::KeyPressed:

                switch (event.key.code)
                {
                case sf::Keyboard::M:

                    if (moveMode){
                        moveMode = false;
                        if (clientCursor.loadFromSystem(sf::Cursor::Arrow))
                            clientGameWindow.setMouseCursor(clientCursor);
                    } 
                    else {
                        moveMode = true;
                        if (clientCursor.loadFromSystem(sf::Cursor::Hand)) 
                            clientGameWindow.setMouseCursor(clientCursor);
                    }
                    break;

                case sf::Keyboard::R:

                    newMapOffset = {MAP_X_OFFSET - firstHexagonPosition[0],
                                    MAP_Y_OFFSET - firstHexagonPosition[1]};

                    firstHexagonPosition = {MAP_X_OFFSET, MAP_Y_OFFSET};

                    for(unsigned i = 0; i < mapTextureToDisplay.size(); i++){
                        mutexGame.lock();
                        mapTextureToDisplay[i].moveSpritePosition(newMapOffset[0], newMapOffset[1]);
                        mutexGame.unlock();
                    }

                    for(auto& kv : elementTextureToDisplay){
                        mutexGame.lock();
                        kv.second->moveSpritePosition(newMapOffset[0], newMapOffset[1]);
                        mutexGame.unlock();
                    }

                    clickStartingPoint = sf::Mouse::getPosition(clientGameWindow);

                    break;

                default:
                    break;
                }
                break;

            case sf::Event::Closed:
                clientGameWindow.close();
                break;

            default:
                break;
            }
        }
    }
}

void GameWindow::clickAction(sf::Vector2i clickPosition, std::function<void(int, int)> callback) {

    int minimumDistance = WINDOW_LENGTH;
    std::array<int, 2> hexagonOnClick = {0, 0};

    sf::Rect cursorRect = mapTextureToDisplay[0].getSprite(0).getGlobalBounds();
    cursorRect.left = clickPosition.x;
    cursorRect.top = clickPosition.y;
    cursorRect.width = 1;
    cursorRect.height = 1;

    bool isClickable = false;

    for(int i = 0; i < NUMBER_OF_FIELD; i++){

        for(unsigned j = 0; j < mapTextureToDisplay[i].getSize(); j++){

            sf::Rect spriteBounds = mapTextureToDisplay[i].getSprite(j).getGlobalBounds();

            if (spriteBounds.intersects(cursorRect)) {

                isClickable = true;

                int distance = sqrt(pow(spriteBounds.left + spriteBounds.width/2 - cursorRect.left, 2) 
                                    + pow(spriteBounds.top + spriteBounds.height/2 - cursorRect.top, 2));

                if (distance < minimumDistance){

                    minimumDistance = distance;
                    hexagonOnClick[1] =  (int)((spriteBounds.top - firstHexagonPosition[1]))/(int)((spriteBounds.height * 3 / 4));
                    hexagonOnClick[0] =  (int)((spriteBounds.left - firstHexagonPosition[0])) /(int)((spriteBounds.width - 1));
                }
            } 
        }
    }
    if (isClickable) callback(hexagonOnClick[0], hexagonOnClick[1]);
}

/*!
* \brief Open JSON File
*/
const auto GameWindow::openJsonFile(std::string path) {
    
    std::ifstream file(RESOURCES_PATH + path);
    // check is file is correctly open
    if (!file.is_open())
    {
        std::cerr << "Error while opening json ressources file" << std::endl;
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
* \brief Display text on the cards
*/
void GameWindow::displayText(std::vector<CardStruct> *cards, std::string title, std::string body, sf::Font *font)  {

    // display the title on the card    
    cards->back().title = (std::unique_ptr<sf::Text>) new sf::Text(title, *font, TITLE_PROPORTION*WINDOW_LENGTH);  
    cards->back().title->setStyle(sf::Text::Bold);
    cards->back().title->setFillColor(sf::Color::Black);
    auto titleSize = cards->back().title->getLocalBounds();
    int xTitleOffset = (cards->back().texture->getWidth() - titleSize.width)/2;
    int xTitlePosition = cards->back().texture->getSprite().getPosition().x + xTitleOffset;
    int yTitlePosition = cards->back().texture->getSprite().getPosition().y;
    cards->back().title->setPosition( xTitlePosition, yTitlePosition);

    // display the body on the card
    cards->back().body = (std::unique_ptr<sf::Text>) new sf::Text(body, *font, 30);

    //to have the text on several lines without exceeding the card
    int countEndLine = 1;
    while(cards->back().body->getLocalBounds().width > cards->back().texture->getWidth()-10){

        for (int i = countEndLine*NBR_CHAR_MAX_PER_LIGNE; i > 0 ; i--){
            if ((char)body[i] == ' ') {
                body.insert(i, "\n");
                countEndLine++;
                break;
            }
        }
        cards->back().body->setString(body);
    }


    cards->back().body->setFillColor(sf::Color::Black);
    int xBodyOffset = BODY_PROPORTION_X*WINDOW_LENGTH;
    int yBodyOffset = BODY_PROPORTION_Y*WINDOW_WIDTH;
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
            mapTextureToDisplay.at((int)mapShared(j, i)->getFieldLevel()).addMapSprite();
            mapTextureToDisplay.at((int)mapShared(j, i)->getFieldLevel()).setSpritePosition(indexSprite, j, i, MAP_X_OFFSET, MAP_Y_OFFSET, {0, 0});
        }
    }
}

/*!
* \brief Load all the textures of the map
*/
void GameWindow::loadElementTexture() 
{
    std::array<int, 2> hexSize = {mapTextureToDisplay.at(0).getWidth(), mapTextureToDisplay.at(0).getHeight()};

    const Json::Value &data = openJsonFile("/img/map/files.json");

    elementTextureToDisplay.clear();

    for(const auto& kv : elementTextureToDisplay){
        elementTextureToDisplay.erase(kv.first);
    }

    for (unsigned index = 0; index < data.size(); ++index)
    {
        std::string path = RESOURCES_PATH + data[index]["path"].asString();

        elementTextureToDisplay[path] = (std::unique_ptr<client::TextureDisplayer>) new TextureDisplayer(path);

        elementTextureToDisplay[path]->addMapSprite();

        elementTextureToDisplay[path]->setSpritePosition(0, data[index]["y"].asInt(), data[index]["x"].asInt(), firstHexagonPosition[0], firstHexagonPosition[1], hexSize);
    }
}

/*!
* \brief Load all the HUD textures
*/

void GameWindow::loadHudTexture()
{

    int rotation = 0;
    int priorityCardIndex = 0;

    backgroundTexture = (std::unique_ptr<TextureDisplayer>) new TextureDisplayer(RESOURCES_PATH "/img/hud/background.png");    
    backgroundTexture->addMapSprite();
    float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(WINDOW_LENGTH));
    backgroundTexture->setHudSpritePosition(backgroundScale, WINDOW_LENGTH, WINDOW_WIDTH, rotation, priorityCardIndex);

    const Json::Value &data = openJsonFile("/img/hud/files.json");

    for (unsigned index = 0; index < data.size(); ++index)
    {
        hudTextureToDisplay.emplace_back(RESOURCES_PATH + data[index]["path"].asString());
        hudTextureToDisplay.back().addMapSprite();
        float scale = data[index]["scale"].asFloat() / (float(hudTextureToDisplay.back().getWidth()) / float(WINDOW_LENGTH));
        hudTextureToDisplay.back().setImageType((HudTextureType)index);
        hudTextureToDisplay.back().setHudSpritePosition(scale, WINDOW_LENGTH, WINDOW_WIDTH, data[index]["rotation"].asInt(), priorityCardIndex);
    }


    // load the priorityCard
    if(!priorityFont.loadFromFile(RESOURCES_PATH "/img/hud/font.otf")) {
        std::cerr << "Font not loaded" << std::endl;
    }

    const Json::Value &priorityData = openJsonFile( "/img/hud/priority-card.json");

    for (unsigned index = 0; index < priorityData.size(); ++index)
    {
        priorityCards.emplace_back();
        priorityCards.back().texture = (std::unique_ptr<client::TextureDisplayer>) new TextureDisplayer(RESOURCES_PATH + priorityData[index]["path"].asString());
        priorityCards.back().texture->addMapSprite();
        float priorityScale = PRIORITY_CARD_PROPORTION / (float(priorityCards.back().texture->getWidth()) / float(WINDOW_LENGTH));
        priorityCards.back().texture->setImageType((HudTextureType)(index + 7)); // +7 to go to the priority cards in the HudTextureType (enum class)
        priorityCards.back().texture->setHudSpritePosition(priorityScale, WINDOW_LENGTH, WINDOW_WIDTH, 0, index);
        priorityCards.back().level = 0;

        displayText(&priorityCards, priorityData[index]["title"].asString(), priorityData[index]["body"][priorityCards.back().level].asString(), &priorityFont);
     
    }

    
    //load the actionCard 

    const Json::Value &actionCardData = openJsonFile( "/img/hud/action-card.json");
    std::vector<int> actionCardOwned = {1,3,7};   // array that will be sent by shared

    for (unsigned index = 0; index < actionCardOwned.size(); ++index)
    {
        actionCardsToDisplay.emplace_back();
        actionCardsToDisplay.back().texture = (std::unique_ptr<client::TextureDisplayer>) new TextureDisplayer(RESOURCES_PATH + actionCardData[actionCardOwned[index]]["path"].asString());
        actionCardsToDisplay.back().texture->addMapSprite();
        float actionScale = ACTION_CARD_PROPORTION / (float(actionCardsToDisplay.back().texture->getWidth()) / float(WINDOW_LENGTH));
        actionCardsToDisplay.back().texture->setImageType((HudTextureType)(actionCardOwned[index] + 11)); // +11 to go to the action cards in the HudTextureType (enum class)
        actionCardsToDisplay.back().texture->setHudSpritePosition(actionScale, WINDOW_LENGTH, WINDOW_WIDTH, 0, index);

        std::string titleCardAction = actionCardData[actionCardOwned[index]]["type"].asString();
        std::string bodyCardAction = actionCardData[actionCardOwned[index]]["body"].asString();

        displayText(&actionCardsToDisplay, titleCardAction, bodyCardAction, &priorityFont);

    }
    
}

long GameWindow::getCurrentTime(bool timeSecond){
    if (timeSecond) return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    else return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

}
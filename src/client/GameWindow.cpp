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
#define NBR_CHAR_MAX_PER_LIGNE 25 
#define TURN_NUMBER 2

#ifndef RESOURCES_PATH
    #define RESOURCES_PATH "../resources"
#endif

namespace client
{

/*!
 * \brief Constructeur
 *
 * Constructor of GameWindow class
 */
GameWindow::GameWindow() {
    clientGameWindow.create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Civilization VII", sf::Style::Close);
    clientGameWindow.setPosition(sf::Vector2i(0, 0));
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

    for (unsigned i = 0; i < priorityCards.size(); i++){
        clientGameWindow.draw(priorityCards[i].texture->getSprite(0));
        clientGameWindow.draw(*priorityCards[i].title);
        clientGameWindow.draw(*priorityCards[i].body);
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
void GameWindow::clientWindow()
{

    int turn = 0;
    int moveMode = false;

    std::array<int, 2> clickStartingPoint = {0, 0};
    std::array<int, 2> newMapOffset = {0, 0};

    while (clientGameWindow.isOpen()){

        // handle events
        sf::Event event;
        while (clientGameWindow.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::MouseButtonPressed:
        
                clickStartingPoint = {  sf::Mouse::getPosition(clientGameWindow).x, 
                                        sf::Mouse::getPosition(clientGameWindow).y};

                if (!moveMode) {

                    int minimumDistance = WINDOW_LENGTH;
                    std::array<int, 2> hexagonOnClick = {0, 0};
                    std::array<int, 2> firstHexagonPosition = {WINDOW_LENGTH, WINDOW_WIDTH};

                    sf::Rect cursorRect = mapTextureToDisplay[0].getSprite(0).getGlobalBounds();
                    cursorRect.left = clickStartingPoint[0];
                    cursorRect.top = clickStartingPoint[1];
                    cursorRect.width = 1;
                    cursorRect.height = 1;

                    bool isClickable = false;

                    for(int i = 0; i < NUMBER_OF_FIELD; i++){

                        for(unsigned j = 0; j < mapTextureToDisplay[i].getSize(); j++){

                            sf::Rect spriteBounds = mapTextureToDisplay[i].getSprite(j).getGlobalBounds();

                            if (firstHexagonPosition[0] > spriteBounds.left) firstHexagonPosition[0] = spriteBounds.left;
                            if (firstHexagonPosition[1] > spriteBounds.top) firstHexagonPosition[1] = spriteBounds.top;

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
                    if (isClickable) std::cout << "User click on the Hex x=" << hexagonOnClick[0] << " & y=" << hexagonOnClick[1] << "\n";
                }

                break;

            case sf::Event::MouseButtonReleased:
            
                if (moveMode){

                    newMapOffset = {sf::Mouse::getPosition(clientGameWindow).x - clickStartingPoint[0],
                                    sf::Mouse::getPosition(clientGameWindow).y - clickStartingPoint[1]};

                    for(unsigned i = 0; i < mapTextureToDisplay.size(); i++)
                        mapTextureToDisplay[i].moveSpritePosition(newMapOffset[0], newMapOffset[1]);

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
            
            // draw the map
            if (turn == 0) {
                loadMapTexture();
                loadHudTexture();
                turn += 1;
            }
            displayWindow();
        }
    }
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
* \brief Load all the textures of the map
*/
void GameWindow::loadMapTexture() 
{

    mapShared.generateRandomMap(123456789);

    std::string hexagonImgPath = RESOURCES_PATH "/img/map/field-";
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

    std::array<int, 2> hexSize = {mapTextureToDisplay.at(0).getWidth(), mapTextureToDisplay.at(0).getHeight()};

    const Json::Value &data = openJsonFile("/img/map/files.json");

    for (unsigned index = 0; index < data.size(); ++index)
    {

        mapTextureToDisplay.emplace_back(RESOURCES_PATH + data[index]["path"].asString());

        mapTextureToDisplay.back().addMapSprite();

        mapTextureToDisplay.back().setSpritePosition(0, data[index]["y"].asInt(), data[index]["x"].asInt(), MAP_X_OFFSET, MAP_Y_OFFSET, hexSize);
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


        // display the title on the card
        priorityCards.back().title = (std::unique_ptr<sf::Text>) new sf::Text(priorityData[index]["text"][0].asString(), priorityFont, TITLE_PROPORTION*WINDOW_LENGTH);
        priorityCards.back().title->setStyle(sf::Text::Bold);
        priorityCards.back().title->setFillColor(sf::Color::Black);
        auto titleSize = priorityCards.back().title->getLocalBounds();
        int xTitleOffset = (priorityCards.back().texture->getWidth() - titleSize.width)/2;
        int xTitlePosition = priorityCards.back().texture->getSprite().getPosition().x + xTitleOffset;
        int yTitlePosition = priorityCards.back().texture->getSprite().getPosition().y;
        priorityCards.back().title->setPosition( xTitlePosition, yTitlePosition);

        // display the body on the card
        priorityCards.back().level = 1;  // function get and set PriorityCardLevel in the future
        std::string body = priorityData[index]["text"][priorityCards.back().level].asString();
        priorityCards.back().body = (std::unique_ptr<sf::Text>) new sf::Text(body, priorityFont, 30);

        //to have the text on several lines without exceeding the card
        int countEndLine = 1;
        while(priorityCards.back().body->getLocalBounds().width > priorityCards.back().texture->getWidth()-10){
            for (int i = countEndLine*NBR_CHAR_MAX_PER_LIGNE; i > 0 ; i--){
                if ((char)body[i] == ' ') {
                    body.insert(i, "\n");
                    countEndLine++;
                    break;
                }
            }
            priorityCards.back().body->setString(body);
        }

        priorityCards.back().body->setFillColor(sf::Color::Black);
        int xBodyOffset = BODY_PROPORTION_X*WINDOW_LENGTH;
        int yBodyOffset = BODY_PROPORTION_Y*WINDOW_WIDTH;
        int xBodyPosition = priorityCards.back().texture->getSprite().getPosition().x + xBodyOffset;
        int yBodyPosition = priorityCards.back().texture->getSprite().getPosition().y + yBodyOffset;
        priorityCards.back().body->setPosition(xBodyPosition, yBodyPosition);        

    }

    /*
    // load the actionCard
    actionCards.emplace_back(RESOURCES_PATH "/img/hud/action-card-army.png");
    actionCards.back().addMapSprite();
    //float barbareWheelScale = float(BARBARE_WHEEL_PROPORTION)/(float(hudTextureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
    actionCards.back().setHudSpritePosition(1, WINDOW_LENGTH, WINDOW_WIDTH, rotation, priorityCardIndex); */
}

}

#include <client.hpp>
#include <iostream>
#include <fstream>
#include <json/json.h>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define LADDER_PROPORTION 0.78
#define TECH_WHEEL_PROPORTION 0.187
#define BARBARE_WHEEL_PROPORTION 0.0956
#define PRIORITY_CARD_PROPORTION 0.144
#define ACTION_CARD_PROPORTION 0.125

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
void GameWindow::displayWindow(int numberTurn) {

    clientGameWindow.clear(sf::Color::Blue);

    clientGameWindow.draw(*backgroundTexture->getSprite(0));

    for(unsigned i = 0; i < mapTextureToDisplay.size(); i++ ){

        for(unsigned j = 0; j < mapTextureToDisplay.at(i).getSize(); j++ ){

            clientGameWindow.draw(*mapTextureToDisplay.at(i).getSprite(j));
        }
    }

    for(unsigned i = 0; i < actionCards.size(); i++ ){

        for(unsigned j = 0; j < actionCards.at(i).getSize(); j++ ){

            clientGameWindow.draw(*actionCards.at(i).getSprite(j));
        }
    }

    for(unsigned i = 0; i < priorityCards.size(); i++ ){

        for(unsigned j = 0; j < priorityCards.at(i).getSize(); j++ ){

            clientGameWindow.draw(*priorityCards.at(i).getSprite(j));
        }
    }

    clientGameWindow.draw(*hudTextureToDisplay.at(numberTurn%5).getSprite(0));
    
    for(unsigned i = 5; i < hudTextureToDisplay.size(); i++ ){

        for(unsigned j = 0; j < hudTextureToDisplay.at(i).getSize(); j++ ){

            clientGameWindow.draw(*hudTextureToDisplay.at(i).getSprite(j));
        }
    }

    clientGameWindow.display();
}

/*!
 * \brief Loop that look for events to happend and call displayWindow()
 */
void GameWindow::clientWindow() {

    int turn = 0;

    bool dragging = false;
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
                dragging = true;
                //clickStartingPoint = {sf::Mouse::getPosition(clientGameWindow).x, sf::Mouse::getPosition(clientGameWindow).y};
                break;

            case sf::Event::MouseButtonReleased:
                dragging = false;
                break;

            case sf::Event::Closed:
                clientGameWindow.close();
                break;

            default:
                break;
            }

            if (dragging == true)
            {
                if (sf::Event::MouseMoved)
                {
                    /*newMapOffset = {MAP_X_OFFSET + sf::Mouse::getPosition(clientGameWindow).x - clickStartingPoint.at(0),
                                    MAP_Y_OFFSET + sf::Mouse::getPosition(clientGameWindow).y - clickStartingPoint.at(1)};

                    clientMap.setOffset(newMapOffset.at(0), newMapOffset.at(1));*/
                }
            }
        }
        
        // draw the map
        if (turn == 0) {
            loadMapTexture();
            loadHudTexture();
            turn += 1;
        }
        displayWindow(turn);
    }
}

/*!
 * \brief Load all the textures of the map
 */
void GameWindow::loadMapTexture() {

    std::array<int, 165> level =
    {
        0, 0, 5, 4, 4, 2, 5, 2, 1, 2, 5, 0, 0, 0, 0,
        0, 0, 0, 5, 0, 1, 5, 5, 5, 5, 1, 1, 0, 0, 0, 
        0, 0, 0, 3, 0, 1, 5, 0, 1, 1, 3, 4, 0, 0, 0, 
        0, 0, 0, 0, 1, 2, 5, 3, 2, 3, 1, 4, 0, 0, 0, 
        0, 0, 0, 0, 4, 5, 5, 2, 3, 0, 3, 2, 0, 0, 0, 
        0, 0, 0, 0, 2, 5, 1, 3, 3, 2, 2, 5, 0, 0, 0, 
        0, 0, 5, 1, 2, 1, 5, 3, 1, 5, 2, 5, 4, 0, 0,
        0, 0, 0, 1, 2, 0, 4, 2, 5, 3, 1, 4, 5, 0, 0, 
        0, 0, 0, 0, 2, 0, 5, 4, 2, 0, 0, 3, 2, 2, 0, 
        0, 0, 0, 0, 1, 1, 5, 3, 0, 5, 2, 4, 2, 2, 0, 
        0, 0, 0, 0, 0, 1, 5, 5, 0, 1, 0, 0, 4, 2, 0
    };

    std::string hexagonImgPath = "../ressources/img/map/field-";
    std::array<std::string, 12> mapField =  {"water", "grassland", "hill", "forest", "desert", "mountain",
                                            "wonder-everest", "wonder-galapagos", "wonder-kilimanjaro",
                                            "wonder-messa", "wonder-pantanal", "wonder-volcanic"
                                            };

    for(unsigned i {0}; i < mapField.size(); i++){

        std::string mapElementPath = hexagonImgPath + mapField.at(i) + ".png";
        mapTextureToDisplay.emplace_back(mapElementPath);        
    }

    for(int i = 0; i < 165; i++){

        int indexSprite = mapTextureToDisplay.at(level[i]).getSize();

        mapTextureToDisplay.at(level[i]).addMapSprite();

        //mapTextureToDisplay.at(mapTexture.at(mapShared(i%15,(int)(i/15))->getFieldLevel())).addMapSprite();

        mapTextureToDisplay.at(level[i]).setSpritePosition(indexSprite, i % 15, i / 15, MAP_X_OFFSET, MAP_Y_OFFSET, {0, 0});

    }

    std::array<int, 2> hexSize = {mapTextureToDisplay.at(0).getWidth(), mapTextureToDisplay.at(0).getHeight()};

    std::ifstream file("../ressources/img/map/files.json");
    // check is file is correctly open
    if (!file.is_open()) {
        std::cout << "Error while opening json ressources file" << std::endl;
        exit(1);
    }
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::unique_ptr<Json::CharReader> reader = std::unique_ptr<Json::CharReader>(Json::CharReaderBuilder().newCharReader());
    Json::Value obj;
    std::string errors;
    reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

    const Json::Value& data = obj["data"];

    for (unsigned index = 0; index < data.size(); ++index) {
        
        mapTextureToDisplay.emplace_back(data[index]["path"].asString());

        mapTextureToDisplay.back().addMapSprite();

        int rank = data[index]["x"].asInt()*15 + data[index]["y"].asInt();

        mapTextureToDisplay.back().setSpritePosition(0, rank % 15, rank / 15, MAP_X_OFFSET, MAP_Y_OFFSET, hexSize); 
    }

}

void GameWindow::loadHudTexture() {

    int offsetLength = 0;
    int rotation = 0; 

    backgroundTexture = new TextureDisplayer("../ressources/img/hud/background.png");
    backgroundTexture->addMapSprite();
    float backgroundScale = 1/(float(backgroundTexture->getWidth())/float(WINDOW_LENGTH));
    backgroundTexture->setHudSpritePosition(backgroundScale, WINDOW_LENGTH, WINDOW_WIDTH, rotation);

   std::ifstream file("../ressources/img/hud/files.json");
    // check is file is correctly open
    if (!file.is_open()) {
        std::cout << "Error while opening json ressources file" << std::endl;
        exit(1);
    }
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::unique_ptr<Json::CharReader> reader = std::unique_ptr<Json::CharReader>(Json::CharReaderBuilder().newCharReader());
    Json::Value obj;
    std::string errors;
    reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

    const Json::Value& data = obj["data"];

    for (unsigned index = 0; index < data.size(); ++index) {

        hudTextureToDisplay.emplace_back(data[index]["path"].asString());

        hudTextureToDisplay.back().addMapSprite();

        float scale = data[index]["scale"].asFloat()/(float(hudTextureToDisplay.back().getWidth())/float(WINDOW_LENGTH));

        hudTextureToDisplay.back().setHudSpritePosition(scale, WINDOW_LENGTH, WINDOW_WIDTH, data[index]["rotation"].asInt()); 
    }

    // load the priorityCard
    priorityCards.emplace_back("../ressources/img/hud/priority-card-army.png");
    priorityCards.back().addMapSprite();
    //float barbareWheelScale = float(BARBARE_WHEEL_PROPORTION)/(float(hudTextureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
    priorityCards.back().setHudSpritePosition(1, WINDOW_LENGTH, WINDOW_WIDTH, rotation); 

    // load the actionCard
    actionCards.emplace_back("../ressources/img/hud/action-card-army.png");
    actionCards.back().addMapSprite();
    //float barbareWheelScale = float(BARBARE_WHEEL_PROPORTION)/(float(hudTextureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
    actionCards.back().setHudSpritePosition(1, WINDOW_LENGTH, WINDOW_WIDTH, rotation); 

}

/*
void PriorityCardDisplay::loadTitle(std::string title, sf::Vector2f position, int xCardSize) {
    
    if(!priorityFont.loadFromFile("../ressources/img/hud/font.otf")){
        std::cout << "font not loaded\n" ;
    }

    std::string upperFirstLetter = title;
    upperFirstLetter[0] = toupper(upperFirstLetter[0]);

    titleCard.setFont(priorityFont);
    titleCard.setString(upperFirstLetter);
    titleCard.setCharacterSize((float(40)/float(1600))*WINDOW_LENGTH);
    titleCard.setStyle(sf::Text::Bold);
    titleCard.setColor(sf::Color::Black);
    sf::Rect titleScale =titleCard.getLocalBounds();
    int xOffset = (xCardSize - titleScale.width)/2 ;
    titleCard.setPosition(position.x + xOffset, position.y);
}
*/

}

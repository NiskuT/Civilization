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
void GameWindow::displayWindow() {

    clientGameWindow.clear(sf::Color::Blue);

    clientGameWindow.draw(background.getSprite());

    for(unsigned i = 0; i < textureToDisplay.size(); i++ ){

        for(unsigned j = 0; j < textureToDisplay.at(i).getSize(); j++ ){

            clientGameWindow.draw(*textureToDisplay.at(i).getSprite(j));

        }
    }
/*
    for (int i = 0; i < 5; i++)
    {
        clientGameWindow.draw(priorityCards.at(i).getSprite());
        clientGameWindow.draw(priorityCards.at(i).titleCard);
    }
    for (int i = 0; i < 3; i++)
    {
        clientGameWindow.draw(actionCards.at(i).getSprite());
    }
*/

    clientGameWindow.display();
}

/*!
* \brief Loop that look for events to happend and call displayWindow()
*/
void GameWindow::clientWindow() 
{

    int turn = 0;

    // Display the background
    background.loadHudData("../ressources/img/hud/background.png", "background");
    background.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);

    // Display priority cards
/*
    std::vector<std::string> cardType = {"army", "culture", "economy", "industry", "science"};
    std::string priorityCardFile = "../ressources/img/hud/priority-card-";
    for (int i = 0; i < 5; i++)
    {
        std::string type = cardType[i];
        std::string priorityFileToLoad = priorityCardFile + type + format;
        priorityCards.at(i).loadHudData(priorityFileToLoad, "priorityCard");
        priorityCards.at(i).updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, i);
        sf::Vector2f priorityCardPosition = priorityCards.at(i).getSprite().getPosition();
        sf::Rect rectPriorityCard = priorityCards.at(i).getSprite().getLocalBounds();
        priorityCards.at(i).loadTitle(type, priorityCardPosition, rectPriorityCard.width);
    }

    // Display action cards
    std::vector<std::string> actionCardOwned = {"army", "player-1", "player-2"};
    std::string actionCardFile = "../ressources/img/hud/action-card-";
    for (int i = 0; i < actionCardOwned.size(); i++)
    {
        std::string actionType = actionCardOwned[i];
        std::string actionCardFileToLoad = actionCardFile + actionType + format;
        actionCards.at(i).loadHudData(actionCardFileToLoad, "actionCard");
        actionCards.at(i).updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, i);
    }
*/

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
            loadTexture();
            loadHud();
            turn += 1;
        }
        displayWindow();
    }
}

void GameWindow::loadTexture() {

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
    std::array<std::string, 12> mapField =   {"water", "grassland", "hill", "forest", "desert", "mountain",
                                            "wonder-everest", "wonder-galapagos", "wonder-kilimanjaro",
                                            "wonder-messa", "wonder-pantanal", "wonder-volcanic"
                                            };

    for(unsigned i {0}; i < mapField.size(); i++){

        std::string mapElementPath = hexagonImgPath + mapField.at(i) + ".png";
        textureToDisplay.emplace_back(mapElementPath);        
    }

    for(int i = 0; i < 165; i++){

        int indexSprite = textureToDisplay.at(level[i]).getSize();

        textureToDisplay.at(level[i]).addMapSprite();

        //textureToDisplay.at(mapTexture.at(mapShared(i%15,(int)(i/15))->getFieldLevel())).addMapSprite();

        textureToDisplay.at(level[i]).setMapSpritePosition(indexSprite, i);

    }

    std::array<int, 2> hexSize = {textureToDisplay.at(0).getWidth(), textureToDisplay.at(0).getHeight()};
    
    Json::Value root;
    std::ifstream json_file("../ressources/img/map/files.json", std::ifstream::binary);
    json_file >> root;

    for (unsigned index = 0; index < root["data"].size(); ++index) {

        textureToDisplay.emplace_back(root["data"][index]["path"].asString());

        textureToDisplay.back().addMapSprite();

        int rank = root["data"][index]["x"].asInt()*15 + root["data"][index]["y"].asInt();

        textureToDisplay.back().setElementSpritePosition(0, rank, hexSize); 
    }

}

void GameWindow::loadHud() {

    int offsetLength = 0;
    int rotation = 0; 

    // load the ladder
    textureToDisplay.emplace_back("../ressources/img/hud/ladder.png");
    textureToDisplay.back().addMapSprite();
    float ladderScale = float(LADDER_PROPORTION)/(float(textureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
    textureToDisplay.back().setHudSpritePosition(ladderScale, WINDOW_LENGTH, WINDOW_WIDTH, rotation); 

    // load the techWheel
    textureToDisplay.emplace_back("../ressources/img/hud/tech-wheel.png");
    textureToDisplay.back().addMapSprite();
    float techWheelScale = float(TECH_WHEEL_PROPORTION)/(float(textureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
    textureToDisplay.back().setHudSpritePosition(techWheelScale, WINDOW_LENGTH, WINDOW_WIDTH, 45); 

    // load the barbareWheel
    textureToDisplay.emplace_back("../ressources/img/hud/barbare-wheel-0.png");
    textureToDisplay.back().addMapSprite();
    float barbareWheelScale = float(BARBARE_WHEEL_PROPORTION)/(float(textureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
    textureToDisplay.back().setHudSpritePosition(barbareWheelScale, WINDOW_LENGTH, WINDOW_WIDTH, 45);

     // load the priorityCard
    textureToDisplay.emplace_back("../ressources/img/hud/priority-card-army.png");
    textureToDisplay.back().addMapSprite();
    //float barbareWheelScale = float(BARBARE_WHEEL_PROPORTION)/(float(textureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
    textureToDisplay.back().setHudSpritePosition(1, WINDOW_LENGTH, WINDOW_WIDTH, rotation); 

    // load the actionCard
    textureToDisplay.emplace_back("../ressources/img/hud/action-card-army.png");
    textureToDisplay.back().addMapSprite();
    //float barbareWheelScale = float(BARBARE_WHEEL_PROPORTION)/(float(textureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
    textureToDisplay.back().setHudSpritePosition(1, WINDOW_LENGTH, WINDOW_WIDTH, rotation); 

   

}

}

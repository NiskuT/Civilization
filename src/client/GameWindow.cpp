#include <client.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <json/json.h>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50
#define MAP_WIDTH 15
#define MAP_HEIGHT 11

#define NUMBER_OF_FIELD 12

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define TURN_NUMBER 2

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

        for(unsigned i = 0; i < mapTextureToDisplay.size(); i++ )

            for(unsigned j = 0; j < mapTextureToDisplay[i].getSize(); j++ )

                clientGameWindow.draw(mapTextureToDisplay[i].getSprite(j));


        clientGameWindow.draw(hudTextureToDisplay.at(TURN_NUMBER%5).getSprite());
        
        for(unsigned i = 5; i < hudTextureToDisplay.size(); i++ )

            for(unsigned j = 0; j < hudTextureToDisplay[i].getSize(); j++ )
            
                clientGameWindow.draw(hudTextureToDisplay[i].getSprite(j));

        clientGameWindow.display();
    }

    /*!
     * \brief Loop that look for events to happend and call displayWindow()
     */
    void GameWindow::clientWindow()
    {

        int turn = 0;
        int mooveMode = false;

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

                    if (!mooveMode) {

                        int minimumDistance = WINDOW_LENGTH;
                        std::array<int, 2> hexagonOnClick = {0, 0};
                        std::array<int, 2> firstHexagon = {WINDOW_LENGTH, WINDOW_WIDTH};

                        sf::Rect cursorRect = mapTextureToDisplay[0].getSprite(0).getGlobalBounds();
                        cursorRect.left = clickStartingPoint[0];
                        cursorRect.top = clickStartingPoint[1];
                        cursorRect.width = 1;
                        cursorRect.height = 1;

                        for(int i = 0; i < NUMBER_OF_FIELD; i++){

                            for(unsigned j = 0; j < mapTextureToDisplay[i].getSize(); j++){

                                sf::Rect spriteBounds = mapTextureToDisplay[i].getSprite(j).getGlobalBounds();

                                if (firstHexagon[0] > spriteBounds.left) firstHexagon[0] = spriteBounds.left;
                                if (firstHexagon[1] > spriteBounds.top) firstHexagon[1] = spriteBounds.top;

                                if (spriteBounds.intersects(cursorRect)) {

                                    int distance = pow( pow(spriteBounds.left + spriteBounds.width/2 - cursorRect.left, 2) 
                                                    + pow(spriteBounds.top + spriteBounds.height/2 - cursorRect.top, 2), 0.5);

                                    if (distance < minimumDistance){

                                        minimumDistance = distance;
                                        hexagonOnClick[1] =  (int)((spriteBounds.top - firstHexagon[1]))/(int)((spriteBounds.height * 3 / 4));
                                        hexagonOnClick[0] =  (int)((spriteBounds.left - firstHexagon[0])) /(int)((spriteBounds.width - 1));
                                    }
                                } 
                            }
                        }
                        std::cout << "User click on the Hex x=" << hexagonOnClick[0] << " & y=" << hexagonOnClick[1] << "\n";
                    }

                    break;

                case sf::Event::MouseButtonReleased:
                
                    if (mooveMode){

                        newMapOffset = {sf::Mouse::getPosition(clientGameWindow).x - clickStartingPoint[0],
                                        sf::Mouse::getPosition(clientGameWindow).y - clickStartingPoint[1]};

                        for(unsigned i = 0; i < mapTextureToDisplay.size(); i++)
                            mapTextureToDisplay[i].mooveSpritePosition(newMapOffset[0], newMapOffset[1]);

                    }

                    break;

                case sf::Event::KeyPressed:

                    switch (event.key.code)
                    {
                    case sf::Keyboard::M:

                        if (mooveMode){
                            mooveMode = false;
                            if (clientCursor.loadFromSystem(sf::Cursor::Arrow))
                                clientGameWindow.setMouseCursor(clientCursor);
                        } 
                        else {
                            mooveMode = true;
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
    * \brief Load all the textures of the map
    */
    void GameWindow::loadMapTexture() 
    {

        mapShared.generateRandomMap(123456789);

        std::string hexagonImgPath = "../ressources/img/map/field-";
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

        std::ifstream file("../ressources/img/map/files.json");
        // check is file is correctly open
        if (!file.is_open())
        {
            std::cout << "Error while opening json ressources file" << std::endl;
            exit(1);
        }
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::unique_ptr<Json::CharReader> reader = std::unique_ptr<Json::CharReader>(Json::CharReaderBuilder().newCharReader());
        Json::Value obj;
        std::string errors;
        reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

        const Json::Value &data = obj["data"];

        for (unsigned index = 0; index < data.size(); ++index)
        {

            mapTextureToDisplay.emplace_back(data[index]["path"].asString());

            mapTextureToDisplay.back().addMapSprite();

            mapTextureToDisplay.back().setSpritePosition(0, data[index]["y"].asInt(), data[index]["x"].asInt(), MAP_X_OFFSET, MAP_Y_OFFSET, hexSize);
        }
    }

    void GameWindow::loadHudTexture() {

        int rotation = 0; 

        backgroundTexture = (std::unique_ptr<TextureDisplayer>) new TextureDisplayer("../ressources/img/hud/background.png");
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
    }
}

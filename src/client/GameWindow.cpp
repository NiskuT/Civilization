#include <client.hpp>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <cmath>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define PRIORITY_CARD_PROPORTION 0.144
#define ACTION_CARD_PROPORTION 0.125
#define TITLE_PROPORTION 0.025
#define BODY_PROPORTION_X 0.0075
#define BODY_PROPORTION_Y 0.05
#define MAX_CHARACTER_SIZE 18
#define NBR_CHAR_MAX_PER_LIGNE 25 




namespace client
{

    /*!
     * \brief Constructeur
     *
     * Constructor of GameWindow class
     */
    GameWindow::GameWindow()
    {
        clientGameWindow.create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Civilization VII", sf::Style::Close);
        clientGameWindow.setPosition(sf::Vector2i(0, 0));
    }

    /*!
     * \brief Display all the different variable in the screen
     */
    void GameWindow::displayWindow(int numberTurn)
    {

        clientGameWindow.clear(sf::Color::Blue);

        clientGameWindow.draw(*backgroundTexture->getSprite(0));

        for (unsigned i = 0; i < mapTextureToDisplay.size(); i++)
        {
            for (unsigned j = 0; j < mapTextureToDisplay[i].getSize(); j++)
            {
                clientGameWindow.draw(*mapTextureToDisplay[i].getSprite(j));
            }
        }

        /*for(unsigned i = 0; i < actionCards.size(); i++ ){

            for(unsigned j = 0; j < actionCards.at(i).getSize(); j++ ){

                clientGameWindow.draw(*actionCards.at(i).getSprite(j));
            }
        }*/


        for (unsigned i = 0; i < priorityCards.size(); i++){
            clientGameWindow.draw(*priorityCards[i].texture->getSprite(0));
            clientGameWindow.draw(*priorityCards[i].title);
            clientGameWindow.draw(*priorityCards[i].body);
        }

        clientGameWindow.draw(*hudTextureToDisplay[numberTurn % 5].getSprite(0));

        for (unsigned i = 5; i < hudTextureToDisplay.size(); i++)
        {

            for (unsigned j = 0; j < hudTextureToDisplay[i].getSize(); j++)
            {

                clientGameWindow.draw(*hudTextureToDisplay[i].getSprite(j));
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

        bool dragging = false;

        while (clientGameWindow.isOpen())
        {

            // handle events
            sf::Event event;
            while (clientGameWindow.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::MouseButtonPressed:
                    dragging = true;
                    // clickStartingPoint = {sf::Mouse::getPosition(clientGameWindow).x, sf::Mouse::getPosition(clientGameWindow).y};
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
                    /*
                    if (sf::Event::MouseMoved)
                    {
                        newMapOffset = {MAP_X_OFFSET + sf::Mouse::getPosition(clientGameWindow).x - clickStartingPoint.at(0),
                                        MAP_Y_OFFSET + sf::Mouse::getPosition(clientGameWindow).y - clickStartingPoint.at(1)};

                        clientMap.setOffset(newMapOffset.at(0), newMapOffset.at(1));
                    }
                    */
                }
            }

            // draw the map
            if (turn == 0)
            {
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
    void GameWindow::loadMapTexture()
    {

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
                0, 0, 0, 0, 0, 1, 5, 5, 0, 1, 0, 0, 4, 2, 0};

        std::string hexagonImgPath = "../ressources/img/map/field-";
        std::array<std::string, 12> mapField = {"water", "grassland", "hill", "forest", "desert", "mountain",
                                                "wonder-everest", "wonder-galapagos", "wonder-kilimanjaro",
                                                "wonder-messa", "wonder-pantanal", "wonder-volcanic"};

        for (unsigned i{0}; i < mapField.size(); i++)
        {

            std::string mapElementPath = hexagonImgPath + mapField[i] + ".png";
            mapTextureToDisplay.emplace_back(mapElementPath);
        }

        for (int i = 0; i < 165; i++)
        {

            int indexSprite = mapTextureToDisplay[level[i]].getSize();

            mapTextureToDisplay[level[i]].addMapSprite();

            // mapTextureToDisplay.at(mapTexture.at(mapShared(i%15,(int)(i/15))->getFieldLevel())).addMapSprite();

            mapTextureToDisplay[level[i]].setSpritePosition(indexSprite, i % 15, i / 15, MAP_X_OFFSET, MAP_Y_OFFSET, {0, 0});
        }

        std::array<int, 2> hexSize = {mapTextureToDisplay[0].getWidth(), mapTextureToDisplay[0].getHeight()};

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

            int rank = data[index]["x"].asInt() * 15 + data[index]["y"].asInt();

            mapTextureToDisplay.back().setSpritePosition(0, rank % 15, rank / 15, MAP_X_OFFSET, MAP_Y_OFFSET, hexSize);
        }
    }

    void GameWindow::loadHudTexture()
    {

        int rotation = 0;
        int priorityCardIndex = 0;

        backgroundTexture = new TextureDisplayer("../ressources/img/hud/background.png");
        backgroundTexture->addMapSprite();
        float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(WINDOW_LENGTH));
        backgroundTexture->setHudSpritePosition(backgroundScale, WINDOW_LENGTH, WINDOW_WIDTH, rotation, priorityCardIndex);

        std::ifstream file("../ressources/img/hud/files.json");
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

            hudTextureToDisplay.emplace_back(data[index]["path"].asString());

            hudTextureToDisplay.back().addMapSprite();

            float scale = data[index]["scale"].asFloat() / (float(hudTextureToDisplay.back().getWidth()) / float(WINDOW_LENGTH));

            hudTextureToDisplay.back().setImageType((HudTextureType)index);

            hudTextureToDisplay.back().setHudSpritePosition(scale, WINDOW_LENGTH, WINDOW_WIDTH, data[index]["rotation"].asInt(), priorityCardIndex);

        }


        // load the priorityCard
        if(!priorityFont.loadFromFile("../ressources/img/hud/font.otf")) {
            std::cout << "Font not loaded\n" ;
        }

        std::ifstream priorityFile("../ressources/img/hud/priority-card.json");
        // check is priorityFile is correctly open
        if (!priorityFile.is_open())
        {
            std::cout << "Error while opening json ressources priorityFile" << std::endl;
            exit(1);
        }
        std::string priorityStr((std::istreambuf_iterator<char>(priorityFile)), std::istreambuf_iterator<char>());

        std::unique_ptr<Json::CharReader> prorityReader = std::unique_ptr<Json::CharReader>(Json::CharReaderBuilder().newCharReader());
        Json::Value priorityObj;
        std::string prorityErrors;
        prorityReader->parse(priorityStr.c_str(), priorityStr.c_str() + priorityStr.size(), &priorityObj, &prorityErrors);

        const Json::Value &priorityData = priorityObj["data"];
        for (unsigned index = 0; index < priorityData.size(); ++index)
        {
            priorityCards.emplace_back();
            priorityCards.back().texture = (std::unique_ptr<client::TextureDisplayer>) new TextureDisplayer(priorityData[index]["path"].asString());
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
            int xTitlePosition = priorityCards.back().texture->sprites[0].getPosition().x + xTitleOffset;
            int yTitlePosition = priorityCards.back().texture->sprites[0].getPosition().y;
            priorityCards.back().title->setPosition( xTitlePosition, yTitlePosition);

            // display the body on the card
            priorityCards.back().level = 1;
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
            int xBodyPosition = priorityCards.back().texture->sprites[0].getPosition().x + xBodyOffset;
            int yBodyPosition = priorityCards.back().texture->sprites[0].getPosition().y + yBodyOffset;
            priorityCards.back().body->setPosition(xBodyPosition, yBodyPosition);        

        }

        /*
        // load the actionCard
        actionCards.emplace_back("../ressources/img/hud/action-card-army.png");
        actionCards.back().addMapSprite();
        //float barbareWheelScale = float(BARBARE_WHEEL_PROPORTION)/(float(hudTextureToDisplay.back().getWidth())/float(WINDOW_LENGTH));
        actionCards.back().setHudSpritePosition(1, WINDOW_LENGTH, WINDOW_WIDTH, rotation, priorityCardIndex); */
    }

}

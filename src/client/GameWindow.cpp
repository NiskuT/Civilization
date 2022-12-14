#include <client.hpp>
#include <iostream>
#include <fstream>
#include <json/json.h>

#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0

namespace client
{

    /*!
     * \brief Constructeur
     *
     * Constructor of GameWindow class
     */
    GameWindow::GameWindow()
    {
        clientGameWindow.create(sf::VideoMode(1231, 725), "Civilization VII");
    }

    /*!
     * \brief Display all the different variable in the screen
     */
    void GameWindow::displayWindow()
    {

        clientGameWindow.clear(sf::Color::Blue);

        for (unsigned i = 0; i < textureToDisplay.size(); i++)
        {

            for (unsigned j = 0; j < textureToDisplay.at(i).getSize(); j++)
            {
                clientGameWindow.draw(textureToDisplay.at(i).getSprite(j));
            }
        }

        clientGameWindow.display();
    }

    /*!
     * \brief Loop that look for events to happend and call displayWindow()
     */
    void GameWindow::clientWindow()
    {

        loadMapTexture();

        while (clientGameWindow.isOpen())
        {
            // handle events
            sf::Event event;
            while (clientGameWindow.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    clientGameWindow.close();
            }
            displayWindow();
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
            textureToDisplay.emplace_back(mapElementPath);
        }

        for (unsigned i = 0; i < mapShared.getMapHeight(); i++)
        {
            for (unsigned j = 0; j < mapShared.getMapWidth(); j++)
            {
                int indexSprite = textureToDisplay.at((int)mapShared(j, i)->getFieldLevel()).getSize();
                textureToDisplay.at((int)mapShared(j, i)->getFieldLevel()).addMapSprite();
                textureToDisplay.at((int)mapShared(j, i)->getFieldLevel()).setSpritePosition(indexSprite, j, i, MAP_X_OFFSET, MAP_Y_OFFSET, {0, 0});
            }

        }

        std::array<int, 2> hexSize = {textureToDisplay.at(0).getWidth(), textureToDisplay.at(0).getHeight()};

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

            textureToDisplay.emplace_back(data[index]["path"].asString());

            textureToDisplay.back().addMapSprite();

            int rank = data[index]["x"].asInt() * 15 + data[index]["y"].asInt();

            textureToDisplay.back().setSpritePosition(0, rank % 15, rank / 15, MAP_X_OFFSET, MAP_Y_OFFSET, hexSize);
        }
    }

}

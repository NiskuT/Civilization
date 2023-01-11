#include <client.hpp>
#include <iostream>
#include <json/json.h>
#include <fstream>

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

#define TITLE_SIZE 200

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
    MenuWindow::MenuWindow()
    {
        loadMenuTexture();
    }

    /*!
     * \brief Display all the different variable in the screen
     */
    void MenuWindow::displayWindow()
    {

        clientMenuWindow->clear(sf::Color::Blue);

        clientMenuWindow->draw(backgroundTexture->getSprite());
        clientMenuWindow->draw(*gameTitle);

        for (unsigned i = 0; i < menuButtons.size(); i++)
        {
            clientMenuWindow->draw(*menuButtons[i].buttonRect);
            clientMenuWindow->draw(*menuButtons[i].buttonText);
        }

        clientMenuWindow->display();
    }

    /*!
     * \brief Loop that look for events to happend and call displayWindow()
     */
    void MenuWindow::startMenu(std::shared_ptr<sf::RenderWindow> clientWindow, std::function<void(bool)> quitGame)
    {
        clientMenuWindow = clientWindow;

        long lastUpdateTimer = getCurrentTime(false);

        while (clientMenuWindow->isOpen())
        {

            if (getCurrentTime(false) - lastUpdateTimer > (100 / 3))
            {
                displayWindow();
                lastUpdateTimer = getCurrentTime(false);
            }

            // handle events
            sf::Event event;
            while (clientMenuWindow->pollEvent(event))
            {

                switch (event.type)
                {
                case sf::Event::MouseButtonPressed:

                    std::cout << "Click \n";

                    break;

                case sf::Event::KeyPressed:

                    switch (event.key.code)
                    {
                    case sf::Keyboard::K:
                        quitGame(false);
                        return;

                    case sf::Keyboard::Escape:
                        quitGame(true);
                        return;

                    default:
                        break;
                    }

                    break;

                case sf::Event::Closed:
                    quitGame(true);
                    return;

                default:
                    break;
                }
            }
        }
    }

    void MenuWindow::loadMenuTexture()
    {

        backgroundTexture = (std::unique_ptr<TextureDisplayer>)new TextureDisplayer(RESOURCES_PATH "/img/menu/background.png");
        backgroundTexture->addSprite();
        float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(WINDOW_LENGTH));
        backgroundTexture->setHudSpritePosition(backgroundScale, WINDOW_LENGTH, WINDOW_WIDTH, 0, 0);

        menuFont = (std::unique_ptr<sf::Font>)new sf::Font();
        if (!menuFont->loadFromFile(RESOURCES_PATH "/img/hud/font.otf"))
            std::cerr << "Font not loaded" << std::endl;

        gameTitle = (std::unique_ptr<sf::Text>)new sf::Text("Civilization 7", *menuFont, TITLE_SIZE);
        gameTitle->setStyle(sf::Text::Bold);
        gameTitle->setFillColor(sf::Color::Black);
        gameTitle->setPosition(WINDOW_LENGTH - gameTitle->getLocalBounds().height - gameTitle->getLocalBounds().width, WINDOW_WIDTH - 2.5 * gameTitle->getLocalBounds().height);

        sf::Color buttonColor = sf::Color(247, 200, 195, 255);

        std::ifstream file(RESOURCES_PATH "/img/menu/menu.json");
        if (!file.is_open())
        {
            std::cerr << "Error while opening json ressources file /img/menu/menu.json" << std::endl;
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
            menuButtons.emplace_back(   sf::Vector2f(data[index]["width"].asFloat() * gameTitle->getLocalBounds().width, data[index]["height"].asFloat() * gameTitle->getLocalBounds().height), 
                                        sf::Vector2f(   gameTitle->getPosition().x + data[index]["x"].asFloat() * gameTitle->getLocalBounds().width, 
                                                        gameTitle->getPosition().y - data[index]["y"].asFloat() * gameTitle->getLocalBounds().height), buttonColor);
            menuButtons.back().setText(40, sf::Vector2f(0, 0), data[index]["text"].asString(), &(*menuFont));
        }
    }

    long MenuWindow::getCurrentTime(bool timeSecond)
    {
        if (timeSecond)
            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        else
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

}
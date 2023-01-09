#include <client.hpp>
#include <iostream>

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

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

        clientMenuWindow->display();
    }

    /*!
     * \brief Loop that look for events to happend and call displayWindow()
     */
    void MenuWindow::startMenu(std::shared_ptr<sf::RenderWindow> clientWindow, std::function<void()> quitGame)
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
                    case sf::Keyboard::Q:

                        quitGame();
                        return;

                        break;

                    default:
                        break;
                    }

                    break;

                case sf::Event::Closed:
                    clientMenuWindow->close();
                    break;

                default:
                    break;
                }
            }
        }
    }

    void MenuWindow::loadMenuTexture(){

        backgroundTexture = (std::unique_ptr<TextureDisplayer>)new TextureDisplayer(RESOURCES_PATH "/img/menu/background.png");
        backgroundTexture->addSprite();
        float backgroundScale = 1 / (float(backgroundTexture->getWidth()) / float(WINDOW_LENGTH));
        backgroundTexture->setHudSpritePosition(backgroundScale, WINDOW_LENGTH, WINDOW_WIDTH, 0, 0);
    }


    long MenuWindow::getCurrentTime(bool timeSecond)
    {
        if (timeSecond)
            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        else
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

}
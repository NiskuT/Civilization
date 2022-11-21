#include <client.hpp>
#include <iostream>

#define MAP_X_OFFSET 175
#define MAP_Y_OFFSET 50

#define WINDOW_LENGTH 1600
#define WINDOW_WIDTH 900

namespace client
{

    GameWindow::GameWindow()
    {
        clientGameWindow.create(sf::VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Civilization VII", sf::Style::Close);
        clientGameWindow.setPosition(sf::Vector2i(0, 0));
        clientMap.setOffset(MAP_X_OFFSET, MAP_Y_OFFSET);
    }

    void GameWindow::displayWindow()
    {
        clientGameWindow.clear();
        clientGameWindow.draw(background.getSprite());
        clientGameWindow.draw(techWheel.getSprite());
        clientGameWindow.draw(barbareWheel.getSprite());
        clientGameWindow.draw(clientMap.hexClientDisplay);
        for (sf::Sprite i : clientMap.elementSprites)
        {
            clientGameWindow.draw(i);
        }
        for (int i = 0; i < 5; i++)
        {
            clientGameWindow.draw(priorityCards.at(i).getSprite());
            clientGameWindow.draw(priorityCards.at(i).titleCard);
        }
        for (int i = 0; i < 3; i++)
        {
            clientGameWindow.draw(actionCards.at(i).getSprite());
        }
        clientGameWindow.draw(ladder.getSprite());

        clientGameWindow.display();
    }

    void GameWindow::gameWindow()
    {

        // Display the background
        background.loadHudData("../ressources/img/hud/background.png", "background");
        background.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);

        // Display ladder
        ladder.loadHudData("../ressources/img/hud/ladder.png", "ladder");
        ladder.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);

        // Display tech wheel

        techWheel.loadHudData("../ressources/img/hud/tech-wheel.png", "techWheel");
        techWheel.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);

        // Display event dial (barbare wheel)

        int posBarbareWheel = 0;
        std::string BarbareWheelFile = "../ressources/img/hud/barbare-wheel-";
        std::string wheelNum = std::to_string(posBarbareWheel);
        std::string format = ".png";
        std::string fileToLoad = BarbareWheelFile + wheelNum + format;
        barbareWheel.loadHudData(fileToLoad, "barbareWheel");
        barbareWheel.updatePlacement(WINDOW_LENGTH, WINDOW_WIDTH, 0);

        // Display priority cards

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
                    std::cout << "Left click: " << sf::Mouse::getPosition(clientGameWindow).x << " " << sf::Mouse::getPosition(clientGameWindow).y << "\n";
                    dragging = true;
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
                        std::cout << "Left click: " << sf::Mouse::getPosition(clientGameWindow).x << " " << sf::Mouse::getPosition(clientGameWindow).y << "\n";
                    }
                }
            }

            // draw the map
            displayWindow();
        }
    }

}

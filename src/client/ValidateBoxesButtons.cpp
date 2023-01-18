#include <client.hpp>
#include <iostream>

const sf::Color BACKGROUND_COLOR = sf::Color(0, 0, 0, 150);

using namespace client;


ValidateBoxesButtons::ValidateBoxesButtons(int windowLength, int windowWidth) {
    blackBackground = std::make_unique<Button>(sf::Vector2f(windowLength, windowWidth), sf::Vector2f(0, 0), BACKGROUND_COLOR, false);
    littleBackground = std::make_unique<Button>(sf::Vector2f(windowLength/4, windowWidth/3), sf::Vector2f(300, 300), BACKGROUND_COLOR, false);
}

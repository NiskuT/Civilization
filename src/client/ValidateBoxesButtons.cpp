#include <client.hpp>
#include <iostream>

const sf::Color BACKGROUND_COLOR = sf::Color(0, 0, 0, 150);

using namespace client;


ValidateBoxesButtons::ValidateBoxesButtons() {
    blackBackground = std::make_unique<Button>(sf::Vector2f(1600, 900), sf::Vector2f(0, 0), BACKGROUND_COLOR, false);
}
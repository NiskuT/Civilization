#include <client.hpp>
#include <iostream>

#define ACTION_INDEX 11
#define PRIORITY_INDEX 7

const sf::Color VALIDATE_BUTTON_COLOR = sf::Color(255, 255, 255, 100);
const std::vector<std::string> typeOfPriorityCards = {"economy", "army", "science", "culture", "industry"};

namespace client
{

    GraphicCard::GraphicCard(const std::string& path, const Json::Value &dataNumber, const float windowLength, const float windowWidth, int index, sf::Font& font)
    {
        texture = std::make_unique<TextureDisplayer>(path);
        texture->addSprite();
        texture->setImageType((HudTextureType)(index + PRIORITY_INDEX)); // +7 to go to the priority cards in the HudTextureType (enum class)
        type = typeOfPriorityCards[index];
        difficulty = index;

        priorityScale = dataNumber["priority-card-proportion"].asFloat() / (float(texture->getWidth()) / windowLength);
        upScaleProportion = dataNumber["priority-card-up-scale"].asFloat();
        yBodyOffset = dataNumber["body-y-proportion"].asFloat()* windowWidth;
        this->windowLength = windowLength;
        this->windowWidth = windowWidth;

        texture->setHudSpritePosition(priorityScale, windowLength, windowWidth, 0, index);

        int validateButtonPosX = texture->getSprite().getPosition().x + dataNumber["validate-button-offset-x"].asInt();
        int validateButtonPosY = texture->getSprite().getPosition().y + dataNumber["validate-button-offset-y"].asInt();

        validateButton = std::make_unique<Button>(
                sf::Vector2f(dataNumber["validate-button-size-x"].asInt() , dataNumber["validate-button-size-y"].asInt()),
                sf::Vector2f(validateButtonPosX, validateButtonPosY),
                VALIDATE_BUTTON_COLOR,
                false);

        validateButton->setText(
            dataNumber["validate-button-size-text"].asInt(), 
            sf::Vector2f(0, dataNumber["validate-button-offset-text-y"].asInt()) , 
            "Play", 
            font);


    }

    GraphicCard::GraphicCard(const std::string& path, float actionProportion, const float windowLength, const float windowWidth, int actionCardNumber, int index)
    {
        texture = std::make_unique<TextureDisplayer>(path);
        texture->addSprite();
        texture->setImageType((HudTextureType)(actionCardNumber + ACTION_INDEX)); // +11 to go to the action cards in the HudTextureType (enum class)

        priorityScale = actionProportion / (float(texture->getWidth()) / windowLength);
        upScaleProportion = 0;
        yBodyOffset = 0;
        this->windowLength = windowLength;
        this->windowWidth = windowWidth;

        texture->setHudSpritePosition(priorityScale, windowLength, windowWidth, 0, index);
    }

    /*!
     * @brief Move up priority cards when we click on it
     */

    void GraphicCard::moveUpPriorityCard()
    {
        int yPos;
        int xPos;
        int xTitlePos;
        int xBodyPosition;

        if (isUp)
        {
            yPos = windowWidth - texture->getHeight() * priorityScale + upScaleProportion * windowWidth;
        }
        else
        {
            yPos = windowWidth - texture->getHeight() * priorityScale;
        }
        isUp=!isUp;

        xPos = texture->getSprite().getPosition().x;
        xTitlePos = title->getPosition().x;
        xBodyPosition = body->getPosition().x;

        texture->getSprite().setPosition(xPos, yPos);
        title->setPosition(xTitlePos, yPos);
        body->setPosition(xBodyPosition, yPos + yBodyOffset);
    }
   
}
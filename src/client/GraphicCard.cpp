#include <client.hpp>
#include <iostream>

namespace client
{

    /*!
     * @brief Constructor for the priority cards
     * @param path path of the image of the priority card
     * @param dataNumber data in a json file
     * @param windowLength size of the window length (default: 1600)
     * @param windowWidth size of the window width (default: 900)
     * @param index number of the priority card
     */
    GraphicCard::GraphicCard(const std::string &path, const Json::Value &dataNumber, const float windowLength, const float windowWidth, int index)
    {
        texture = std::make_unique<TextureDisplayer>(path);
        texture->addSprite();
        texture->setImageType((HudTextureType)(index + 7)); // +7 to go to the priority cards in the HudTextureType (enum class)

        priorityScale = dataNumber["priority-card-proportion"].asFloat() / (float(texture->getWidth()) / windowLength);
        upScaleProportion = dataNumber["priority-card-up-scale"].asFloat();
        yBodyOffset = dataNumber["body-y-proportion"].asFloat() * windowWidth;
        this->windowLength = windowLength;
        this->windowWidth = windowWidth;

        texture->setHudSpritePosition(priorityScale, windowLength, windowWidth, 0, index);
    }

    /*!
     * @brief Constructor for the action cards
     * @param path path of the image of the action card
     * @param actionProportion use to set the scale of the action card (usefull to change the window length)
     * @param windowLength size of the window length (default: 1600)
     * @param windowWidth size of the window width (default: 900)
     * @param actionCardNumber number of the action card in all the action card possible
     * @param index index of the action card
     */
    GraphicCard::GraphicCard(const std::string &path, float actionProportion, const float windowLength, const float windowWidth, int actionCardNumber, int index)
    {
        texture = std::make_unique<TextureDisplayer>(path);
        texture->addSprite();
        texture->setImageType((HudTextureType)(actionCardNumber + 11)); // +11 to go to the action cards in the HudTextureType (enum class)

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
        isUp = !isUp;

        xPos = texture->getSprite().getPosition().x;
        xTitlePos = title->getPosition().x;
        xBodyPosition = body->getPosition().x;

        texture->getSprite().setPosition(xPos, yPos);
        title->setPosition(xTitlePos, yPos);
        body->setPosition(xBodyPosition, yPos + yBodyOffset);
    }
}
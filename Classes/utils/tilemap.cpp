#include "utils/tilemap.h"

USING_NS_CC;

Player* Player::createPlayer()
{
    Player* player = Player::create();
    return player;
}

bool Player::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // Set initial speed and direction
    speed = 200.0f; // Adjust the speed as needed
    direction = Vec2::ZERO;

    // Set the player sprite (replace "player.png" with your sprite file)
    this->setTexture("player.png");

    // Schedule update function
    this->scheduleUpdate();

    // Set up keyboard event listeners
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    eventListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    return true;
}

void Player::update(float delta)
{
    // Update player position based on direction and speed
    this->setPosition(this->getPosition() + direction * speed * delta);
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_W:
        direction.y = 1;
        break;
    case EventKeyboard::KeyCode::KEY_S:
        direction.y = -1;
        break;
    case EventKeyboard::KeyCode::KEY_A:
        direction.x = -1;
        break;
    case EventKeyboard::KeyCode::KEY_D:
        direction.x = 1;
        break;
    default:
        break;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_S:
        direction.y = 0;
        break;
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
        direction.x = 0;
        break;
    default:
        break;
    }
}

Vec2 Player::getDirection() const
{
    return direction;
}

float Player::getSpeed() const
{
    return speed;
}

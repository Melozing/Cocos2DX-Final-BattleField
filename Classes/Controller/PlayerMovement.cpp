#include "Controller/PlayerMovement.h"
#include "Constants/Constants.h"

USING_NS_CC;

PlayerMovement::PlayerMovement()
    : _velocity(Vec2::ZERO),
    _speed(Constants::PlayerSpeed),
    _isMoving(false)
{
}

PlayerMovement::~PlayerMovement()
{
}

bool PlayerMovement::init()
{
    if (!cocos2d::Sprite::init())
    {
        return false;
    }

    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerMovement::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerMovement::onKeyReleased, this);
    cocos2d::Sprite::_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Schedule update method
    this->scheduleUpdate();

    return true;
}

void PlayerMovement::update(float delta)
{
    Vec2 position = this->getPosition();
    position += _velocity * _speed * delta;
    this->setPosition(position);
}

void PlayerMovement::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_W:
        moveUp();
        break;
    case EventKeyboard::KeyCode::KEY_S:
        moveDown();
        break;
    case EventKeyboard::KeyCode::KEY_A:
        moveLeft();
        break;
    case EventKeyboard::KeyCode::KEY_D:
        moveRight();
        break;
    default:
        break;
    }
}

void PlayerMovement::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_S:
        stopVerticalMovement();
        break;
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
        stopHorizontalMovement();
        break;
    default:
        break;
    }
}

void PlayerMovement::moveUp()
{
    _velocity.y = 1;
    _isMoving = true;
}

void PlayerMovement::moveDown()
{
    _velocity.y = -1;
    _isMoving = true;
}

void PlayerMovement::moveLeft()
{
    _velocity.x = -1;
    _isMoving = true;
}

void PlayerMovement::moveRight()
{
    _velocity.x = 1;
    _isMoving = true;
}

void PlayerMovement::stopVerticalMovement()
{
    _velocity.y = 0;
    if (_velocity.x == 0)
    {
        _isMoving = false;
    }
}

void PlayerMovement::stopHorizontalMovement()
{
    _velocity.x = 0;
    if (_velocity.y == 0)
    {
        _isMoving = false;
    }
}

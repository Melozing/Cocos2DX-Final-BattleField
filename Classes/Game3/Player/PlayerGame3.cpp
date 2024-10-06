#include "PlayerGame3.h"
#include "Constants/Constants.h"
#include "cocos2d.h"

USING_NS_CC;

PlayerGame3::PlayerGame3()
    : _velocity(Vec2::ZERO),
    _speed(Constants::PlayerSpeed),
    _isMoving(false)
{
}

PlayerGame3::~PlayerGame3()
{
    CC_SAFE_RELEASE(idleAnimate);
    CC_SAFE_RELEASE(moveAnimate);
}

PlayerGame3* PlayerGame3::createPlayerGame3()
{
    PlayerGame3* player = new (std::nothrow) PlayerGame3();
    if (player && player->init())
    {
        player->autorelease();
        player->initAnimation();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool PlayerGame3::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    this->setPosition(Vec2(Constants::InitialPosX, Constants::GroundY));
    this->setScale(Constants::PlayerScale3);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame3::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame3::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Schedule update method
    this->scheduleUpdate();

    return true;
}

void PlayerGame3::initAnimation()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/tank.plist");
    
    modelCharac = Sprite::createWithSpriteFrameName("tank_1.png");
    this->addChild(modelCharac);
   

    // Create idle animation (only needs 8 frames)
    auto idleAnimation = createAnimation("tank_", 30, 0.1f);
    idleAnimate = Animate::create(idleAnimation);
    idleAnimate->retain();

    // Create move animation (8 frames)
    auto moveAnimation = createAnimation("tank_", 30, 0.07f);
    moveAnimate = Animate::create(moveAnimation);
    moveAnimate->retain();

    // Start with idle animation
    startIdleAnimation();
}

Animation* PlayerGame3::createAnimation(const std::string& name, int frameCount, float delay)
{
    Vector<SpriteFrame*> animFrames;
    for (int i = 1; i <= frameCount; ++i) // Only iterate over available frames
    {
        std::string frameName = name + std::to_string(i) + ".png";
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }
    return Animation::createWithSpriteFrames(animFrames, delay);
}

void PlayerGame3::startMovementAnimation()
{
    if (!modelCharac->getActionByTag(1)) // Check if no animation is running
    {
        modelCharac->stopAllActions();
        auto repeatAnimate = RepeatForever::create(moveAnimate);
        repeatAnimate->setTag(1);
        modelCharac->runAction(repeatAnimate);
    }
}

void PlayerGame3::startIdleAnimation()
{
    if (!modelCharac->getActionByTag(1)) // Check if no animation is running
    {
        modelCharac->stopAllActions();
        auto repeatAnimate = RepeatForever::create(idleAnimate);
        repeatAnimate->setTag(1);
        modelCharac->runAction(repeatAnimate);
    }
}

void PlayerGame3::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
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

void PlayerGame3::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
        stopMovement();
        break;
    default:
        break;
    }
}

void PlayerGame3::moveLeft()
{
    _velocity.x = -2;
    _isMoving = true;
    startMovementAnimation();
}

void PlayerGame3::moveRight()
{
    _velocity.x = 2;
    _isMoving = true;
    startMovementAnimation();
}

void PlayerGame3::stopMovement()
{
    _velocity.x = 0;
    _isMoving = false;
    startIdleAnimation();
}

void PlayerGame3::update(float delta)
{
    Vec2 position = this->getPosition();
    position += _velocity * _speed * delta;
    this->setPosition(position);

    // Only start animations when there's a change in movement
    if (_isMoving && !modelCharac->getActionByTag(1))
    {
        startMovementAnimation();
    }
    else if (!_isMoving && !modelCharac->getActionByTag(1))
    {
        startIdleAnimation();
    }
}

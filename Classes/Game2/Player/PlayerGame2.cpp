﻿// PlayerGame2.cpp
#include "PlayerGame2.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "cocos2d.h"
#include "Bullet/Bullet.h"

USING_NS_CC;

PlayerGame2::PlayerGame2()
    : _mousePos(Vec2::ZERO),
    _velocity(Vec2::ZERO),
    _speed(Constants::PlayerSpeed),
    _isMoving(false),
    _isMouseDown(false),
    bulletManager(nullptr)
{
}

PlayerGame2::~PlayerGame2()
{
    delete bulletManager;
}

PlayerGame2* PlayerGame2::createPlayerGame2()
{
    PlayerGame2* player = new (std::nothrow) PlayerGame2();
    player->setName("PlayerGame2");
    if (player && player->init())
    {
        player->autorelease();
        player->initAnimation();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool PlayerGame2::init() {
    if (!Sprite::init()) {
        return false;
    }

    this->setPosition(Vec2(Constants::InitialPosX, Constants::InitialPosY));
    this->setScale(Constants::PlayerScale);
    this->setAnchorPoint(Vec2(0.5, 0.5)); // Set anchor point at the head of the character

    auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setGravityEnable(false); // Disable gravity
    this->setPhysicsBody(physicsBody);

    // Add mouse event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PlayerGame2::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(PlayerGame2::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(PlayerGame2::onMouseUp, this); // Thêm sự kiện onMouseUp
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame2::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Schedule update method
    this->scheduleUpdate();

    // Khởi tạo BulletManager
    bulletManager = new BulletManager(100, "assets_game/player/shot.png");

    return true;
}


void PlayerGame2::initAnimation()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/walkriffle.plist");

    auto spriteBatchNode = SpriteBatchNode::create("assets_game/player/walkriffle.png");
    this->addChild(spriteBatchNode);

    modelCharac = Sprite::createWithSpriteFrameName("walkriffle0.png");
    SpriteController::updateSpriteScale(modelCharac, 0.05f);
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.25f));

    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation("walkriffle", 5, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void PlayerGame2::startMovementAnimation()
{
    if (!this->getActionByTag(1))
    {
        auto animateCharac = Animate::create(createAnimation("walkriffle", 5, 0.07f));
        auto repeatAnimate = RepeatForever::create(animateCharac);
        repeatAnimate->setTag(1);
        modelCharac->runAction(repeatAnimate);
    }
}

void PlayerGame2::onMouseMove(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // Invert the y-coordinate
    auto winSize = Director::getInstance()->getWinSize();
    _mousePos.y = winSize.height - _mousePos.y;
}

void PlayerGame2::onMouseDown(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT || e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        _isMouseDown = true; // Đánh dấu rằng chuột đã được nhấn
    }
}

void PlayerGame2::onMouseUp(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if ((e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT || e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) && _isMouseDown)
    {
        // Chuyển đổi vị trí chuột sang tọa độ OpenGL
        auto mousePos = Director::getInstance()->convertToGL(_mousePos);
        Vec2 pos = this->getPosition();

        // Tính toán hướng bắn
        Vec2 dirToShoot = mousePos - pos;

        // Bắn viên đạn
        shootBullet(dirToShoot);

        // Đặt lại trạng thái chuột
        _isMouseDown = false;
    }
}

void PlayerGame2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
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

void PlayerGame2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
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

void PlayerGame2::moveUp()
{
    _velocity.y = 1;
    _isMoving = true;
}

void PlayerGame2::moveDown()
{
    _velocity.y = -1;
    _isMoving = true;
}

void PlayerGame2::moveLeft()
{
    _velocity.x = -1;
    _isMoving = true;
}

void PlayerGame2::moveRight()
{
    _velocity.x = 1;
    _isMoving = true;
}

void PlayerGame2::stopVerticalMovement()
{
    _velocity.y = 0;
    if (_velocity.x == 0)
    {
        _isMoving = false;
    }
}

void PlayerGame2::stopHorizontalMovement()
{
    _velocity.x = 0;
    if (_velocity.y == 0)
    {
        _isMoving = false;
    }
}

void PlayerGame2::update(float delta)
{
    Vec2 position = this->getPosition();
    position += _velocity * _speed * delta;
    this->setPosition(position);
    RotateToMouse();
    if (_isMoving)
    {
        startMovementAnimation();
    }
    else
    {
        modelCharac->stopActionByTag(1);
    }
}

void PlayerGame2::RotateToMouse()
{
    auto mousePos = Director::getInstance()->convertToGL(_mousePos);
    Vec2 pos = this->getPosition();
    Vec2 dirToFace = mousePos - pos;
    dirToFace.normalize();
    float angle = MathFunction::GetDirInDegreesPositive(dirToFace);
    this->setRotation(-angle + 90);
}

void PlayerGame2::shootBullet(const Vec2& direction)
{
    CCLOG("shootBullet called");

    Vec2 normalizedDirection = direction.getNormalized();
    bulletManager->SpawnBullet(this->getPosition(), normalizedDirection, Constants::BulletSpeed);
}
void PlayerGame2::die()
{
    this->removeFromParent();
    // Add any additional logic for player death, such as game over screen
}

#include "PlayerGame3.h"
#include "Constants/Constants.h"
#include "Bullet/Bullet.h"
#include "Bullet/BulletPool.h"
#include "utils/MathFunction.h"
#include "Manager/PlayerMovementManager.h"
#include "Controller/GameController.h"
#include "cocos2d.h"

USING_NS_CC;

PlayerGame3::PlayerGame3()
    : bulletManager(nullptr),
    playerMovement(nullptr)
{
}

PlayerGame3::~PlayerGame3()
{
    delete bulletManager;
    delete playerMovement;
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

    // Create turret sprite and add to tank
    turretSprite = Sprite::create("assets_game/player/tank_barrel_2.png");
    if (turretSprite) {
        turretSprite->setAnchorPoint(Vec2(0.5, 0.0f)); // Center-bottom
        turretSprite->setPosition(Vec2((this->getContentSize().width / 2) + 10, this->getContentSize().height + 50.0f));
        this->addChild(turretSprite, Constants::ORDER_LAYER_PLAYER);
    }
    else {
        CCLOG("Failed to load texture for Turret");
    }

    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame3::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame3::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Add mouse event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PlayerGame3::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(PlayerGame3::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // Schedule update method
    this->scheduleUpdate();

    // Initialize BulletManager
    bulletManager = new BulletManager(100, "assets_game/player/1.png");
    playerMovement = new PlayerMovement(this, Constants::PLAYER_SPEED_GAME3);
    return true;
}

void PlayerGame3::initAnimation()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/tank.plist");

    modelCharac = Sprite::createWithSpriteFrameName("tank_1.png");
    this->addChild(modelCharac, Constants::ORDER_LAYER_PLAYER);

    auto animateCharac = Animate::create(createAnimation("tank_", 8, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void PlayerGame3::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D)
    {
        playerMovement->onKeyPressed(keyCode);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
    {
        shootBullet();
    }
}

void PlayerGame3::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D)
    {
        playerMovement->onKeyReleased(keyCode);
    }
}

void PlayerGame3::onMouseDown(Event* event)
{
    if (GameController::getInstance()->isPaused())
    {
        return;
    }
    static bool isMouseDown = false;

    if (isMouseDown) return; // Prevent multiple calls
    isMouseDown = true;

    EventMouse* e = (EventMouse*)event;
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        shootBullet();
    }

    this->scheduleOnce([&](float) { isMouseDown = false; }, 0.1f, "resetMouseDownFlag");
}

void PlayerGame3::shootBullet()
{
    // Calculate the local position of the anchor point (0.5f, 1.0f) in the turret's coordinate system
    Vec2 localAnchorPoint = Vec2(turretSprite->getContentSize().width * 0.5f, turretSprite->getContentSize().height * 1.0f);

    // Convert the local anchor point to world position
    Vec2 turretWorldPos = turretSprite->convertToWorldSpace(localAnchorPoint);

    // Calculate direction from turret to mouse position
    Vec2 direction = _mousePos - turretWorldPos;
    direction.normalize();

    // Create bullet sprite and set its properties
    Bullet* bullet = Bullet::createBullet("assets_game/player/1.png", direction, Constants::BulletGame3Speed);
    bullet->setScale(SpriteController::updateSpriteScale(bullet, 0.07f));

    if (bullet)
    {
        bullet->setPosition(turretWorldPos);
        bullet->setDirection(direction);
        bullet->setSpeed(Constants::BulletGame3Speed);
        bullet->activate();

        this->getParent()->addChild(bullet, Constants::ORDER_LAYER_CHARACTER - 5);

        // Move bullet indefinitely
        bullet->moveIndefinitely();
    }
    else
    {
        CCLOG("Failed to create bullet");
    }
}

void PlayerGame3::onMouseMove(Event* event)
{
    if (GameController::getInstance()->isPaused())
    {
        return;
    }

    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());

    updateTurretRotation();
}

void PlayerGame3::update(float delta)
{
    playerMovement->update(delta);

    bulletManager->Update(delta);

    updateTurretRotation();
}

void PlayerGame3::updateTurretRotation()
{
    if (turretSprite)
    {
        // Get the turret's world position
        Vec2 turretPosition = this->convertToWorldSpace(turretSprite->getPosition());

        // Calculate the direction vector from the turret to the mouse position
        Vec2 direction = _mousePos - turretPosition;

        // Normalize the direction vector
        direction.normalize();

        // Calculate the angle in degrees
        float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));

        // Set the turret's rotation (adjusting for the initial orientation)
        turretSprite->setRotation(-angle + 90);
    }
}


void PlayerGame3::setTurretRotation(float angle)
{
    if (turretSprite)
    {
        // Adjust the angle so that the top of the turret points towards the target
        turretSprite->setRotation(-angle + 90);
    }
}

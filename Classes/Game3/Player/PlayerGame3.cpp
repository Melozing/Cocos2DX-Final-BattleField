// PlayerGame3.cpp
#include "PlayerGame3.h"
#include "Constants/Constants.h"
#include "Bullet/Bullet.h"
#include "cocos2d.h"

USING_NS_CC;

PlayerGame3::PlayerGame3()
    : _velocity(Vec2::ZERO),
    _speed(Constants::PlayerSpeed),
    _isMoving(false),
    bulletManager(nullptr)
{
}

PlayerGame3::~PlayerGame3()
{
    delete bulletManager;
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
        turretSprite->setAnchorPoint(Vec2(0.5, 0.13)); // Center-bottom
        turretSprite->setPosition(Vec2((this->getContentSize().width / 2) + 10, this->getContentSize().height + 50.0f));
        this->addChild(turretSprite);
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

    return true;
}

void PlayerGame3::initAnimation()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/tank.plist");

    modelCharac = Sprite::createWithSpriteFrameName("tank_1.png");
    this->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation("tank_", 30, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
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
    case EventKeyboard::KeyCode::KEY_SPACE:
        shootBullet();
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
}

void PlayerGame3::moveRight()
{
    _velocity.x = 2;
    _isMoving = true;
}

void PlayerGame3::stopMovement()
{
    _velocity.x = 0;
    _isMoving = false;
}

void PlayerGame3::movePlayer(const Vec2& direction) {
    // Move player in the specified direction
    this->setPosition(this->getPosition() + direction);

    // Update turret position whenever the tank moves
    this->updateTurret();
}

void PlayerGame3::updateTurret() {
    // Ensure the turret moves with the tank
    if (turretSprite) {
        turretSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height));
    }
    if (turretSprite) {
        float offsetY = 60.0f; // Equivalent to 1 cm in pixels
        turretSprite->setPosition(Vec2(this->getContentSize().width, this->getContentSize().height + offsetY));
    }
    // Update turret rotation based on mouse position or target position
    Vec2 targetPosition = Vec2(/** get target or mouse position **/);
    updateTurretRotation(targetPosition);

    // Calculate angle from tank to mouse position
    Vec2 direction = _mousePos - this->getPosition();
    float angle = direction.getAngle(); // Get angle between movement direction and X-axis
    angle = CC_RADIANS_TO_DEGREES(angle); // Convert from radians to degrees

    // Rotate turret
    setTurretRotation(angle);
}

void PlayerGame3::updateTurretRotation(const Vec2& targetPosition)
{
    if (turretSprite)
    {
        Vec2 direction = targetPosition - this->getPosition();
        float angle = direction.getAngle();
        angle = CC_RADIANS_TO_DEGREES(angle);

        // Adjust the angle to be in the range [0, 180] degrees
        if (angle < 0.0f) angle += 180.0f;
        if (angle > 180.0f) angle = 180.0f;

        // Rotate the turret
        setTurretRotation(angle);
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

void PlayerGame3::onMouseDown(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        CCLOG("Mouse left button clicked");
        shootBullet();
    }
}

void PlayerGame3::shootBullet()
{
    CCLOG("shootBullet called");

    // Calculate the direction from the turret to the mouse position
    Vec2 direction = _mousePos - this->getPosition();
    direction.normalize();

    // Calculate the position of the turret's tip
    float turretAngle = CC_DEGREES_TO_RADIANS(turretSprite->getRotation() - 90); // Adjust for the sprite's rotation
    Vec2 turretTip = turretSprite->getPosition() + Vec2(cos(turretAngle), sin(turretAngle)) * turretSprite->getContentSize().height;

    // Convert turretTip to world coordinates
    Vec2 worldTurretTip = turretSprite->convertToWorldSpace(Vec2(turretSprite->getContentSize().width / 2, turretSprite->getContentSize().height));

    // Use BulletManager to spawn bullet
    bulletManager->SpawnBullet(worldTurretTip, direction, 1600.0f);
}

void PlayerGame3::onMouseMove(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // Invert the y-coordinate
    auto winSize = Director::getInstance()->getWinSize();
    _mousePos.y = winSize.height - _mousePos.y;

    CCLOG("Mouse moved to position: (%f, %f)", _mousePos.x, _mousePos.y);
}

void PlayerGame3::update(float delta)
{
    Vec2 position = this->getPosition();
    position += _velocity * _speed * delta;
    this->setPosition(position);

    // Update BulletManager
    bulletManager->Update(delta);

    // Update turret rotation
    updateTurretRotation(_mousePos);
}



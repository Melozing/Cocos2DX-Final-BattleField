#include "PlayerGame3.h"
#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "Manager/PlayerMovementManager.h"
#include "Manager/ObjectPoolGame3.h"
#include "Controller/GameController.h"
#include "Game3/Enemy/EnemyPlaneBoss.h"
#include "utils/PhysicsShapeCache.h"
#include "cocos2d.h"

USING_NS_CC;

PlayerGame3* PlayerGame3::createPlayerGame3()
{
    PlayerGame3* player = new (std::nothrow) PlayerGame3();
    if (player && player->init())
    {
        player->autorelease();
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

    setupInitialPosition();
    setupTurret();
    setupWheelsAndHandle();
    setupEventListeners();
    setupManagers();
    initAnimation();
    // Schedule update method
    this->scheduleUpdate();

    // Initialize shooting variables
    isMouseDown = false;
    shootDelay = 0.15f;
    timeSinceLastShot = 0.0f;
    bulletCount = 1;

    // Initialize movement and shooting state
    isMovementAndShootingDisabled = false;

    // Preload shoot sound effect
    Constants::QuantityBulletPlayerGame3 = 100;

    return true;
}

void PlayerGame3::setMovementAndShootingDisabled(bool disabled)
{
    isMovementAndShootingDisabled = disabled;
}

void PlayerGame3::setupInitialPosition()
{
    // Get the visible size of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Calculate the initial position based on the padding
    float initialPosX = visibleSize.width / 2; // Center horizontally
    float initialPosY = SpriteController::calculateScreenHeightRatio(0.14f); // Padding from the bottom

    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        initialPosY = SpriteController::calculateScreenHeightRatio(0.09f);
    }

    // Set the initial position of the player
    this->setPosition(Vec2(initialPosX, initialPosY));
    this->setAnchorPoint(Vec2(0.5, 0.5));
}

void PlayerGame3::setupWheelsAndHandle()
{
    // Create left wheel sprite and add to player
    leftWheelSprite = Sprite::create("assets_game/player/LeftWheel.png");
    if (leftWheelSprite) {
        leftWheelSprite->setScale(SpriteController::updateSpriteScale(leftWheelSprite, 0.035f));
        leftWheelSprite->setAnchorPoint(Vec2(0.5, 0.5));
        leftWheelSprite->setPosition(Vec2(this->getContentSize().width - SpriteController::calculateScreenRatio(0.02), this->getContentSize().height - SpriteController::calculateScreenHeightRatio(0.02)));
        this->addChild(leftWheelSprite, Constants::ORDER_LAYER_PLAYER + 1);
    }
    else {
        CCLOG("Failed to load texture for Left Wheel");
    }

    // Create right wheel sprite and add to player
    rightWheelSprite = Sprite::create("assets_game/player/RightWheel.png");
    if (rightWheelSprite) {
        rightWheelSprite->setScale(SpriteController::updateSpriteScale(rightWheelSprite, 0.035f));
        rightWheelSprite->setAnchorPoint(Vec2(0.5, 0.5));
        rightWheelSprite->setPosition(Vec2(this->getContentSize().width + SpriteController::calculateScreenRatio(0.02), this->getContentSize().height - SpriteController::calculateScreenHeightRatio(0.02)));
        this->addChild(rightWheelSprite, Constants::ORDER_LAYER_PLAYER + 1);
    }
    else {
        CCLOG("Failed to load texture for Right Wheel");
    }

    // Create handle sprite and add to player
    handleSprite = Sprite::create("assets_game/player/Circle.png");
    if (handleSprite) {
        handleSprite->setScale(SpriteController::updateSpriteScale(handleSprite, 0.019f));
        handleSprite->setAnchorPoint(Vec2(0.5, 0.5));
        handleSprite->setPosition(Vec2(this->getContentSize().width - SpriteController::calculateScreenRatio(0.005), this->getContentSize().height + SpriteController::calculateScreenHeightRatio(0.02)));
        this->addChild(handleSprite, Constants::ORDER_LAYER_PLAYER + 1);
    }
    else {
        CCLOG("Failed to load texture for Handle");
    }
}


void PlayerGame3::setupTurret()
{
    if (GameController::getInstance()->isGameOver() || GameController::getInstance()->isPaused()) return;
    // Limit the rotation angle to prevent the turret from pointing downwards
    minAngle = 10.0f; // Minimum angle limit
    maxAngle = 170.0f;  // Maximum angle limit

    // Create turret sprite and add to tank
    turretSprite = Sprite::create("assets_game/player/tank_barrel_2.png");
    if (turretSprite) {
        turretSprite->setScale(SpriteController::updateSpriteScale(turretSprite, 0.079f));
        turretSprite->setAnchorPoint(Vec2(0.35, 0.3f)); // Center-bottom
        turretSprite->setPosition(Vec2((this->getContentSize().width / 2) - SpriteController::calculateScreenRatio(0.01f), this->getContentSize().height + SpriteController::calculateScreenRatio(0.016f)));
        this->addChild(turretSprite, Constants::ORDER_LAYER_PLAYER - 1);
    }
    else {
        CCLOG("Failed to load texture for Turret");
    }
}

void PlayerGame3::setupEventListeners()
{
    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame3::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame3::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Add mouse event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PlayerGame3::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(PlayerGame3::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(PlayerGame3::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

bool PlayerGame3::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    _touchPos = touch->getLocation();
    updateTurretRotation();
    return true; 
}

void PlayerGame3::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    _touchPos = touch->getLocation();
    updateTurretRotation();
}

void PlayerGame3::setupManagers()
{
    playerMovement = new PlayerMovement(this, Constants::PLAYER_SPEED_GAME3);
}

void PlayerGame3::initAnimation() {
    // Create idle sprite batch node
    idleBatchNode = SpriteBatchNode::create("assets_game/player/PlayerIdle.png");
    if (idleBatchNode->getParent() == nullptr) {
        this->addChild(idleBatchNode, Constants::ORDER_LAYER_PLAYER);
    }

    // Create idle sprite
    idleSprite = Sprite::createWithSpriteFrameName("PlayerIdle0.png");
    idleSprite->setScale(SpriteController::updateSpriteScale(idleSprite, Constants::PlayerScale3));
    idleBatchNode->addChild(idleSprite, Constants::ORDER_LAYER_PLAYER);

    // Run animations
    auto animateIdle = Animate::create(SpriteController::createAnimation("PlayerIdle", 39, 0.033f));
    idleSprite->runAction(RepeatForever::create(animateIdle));
    this->createPhysicsBody();
}

void PlayerGame3::rotateSpritesContinuously(float angle)
{
    // Create rotation actions
    auto rotate = RotateBy::create(0.1f, angle);
    auto repeatRotation = RepeatForever::create(rotate);

    // Run actions on sprites
    leftWheelSprite->runAction(repeatRotation->clone());
    rightWheelSprite->runAction(repeatRotation->clone());
    handleSprite->runAction(repeatRotation->clone());
}

void PlayerGame3::stopRotatingSprites()
{
    // Stop all rotation actions
    leftWheelSprite->stopAllActions();
    rightWheelSprite->stopAllActions();
    handleSprite->stopAllActions();
}

void PlayerGame3::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D
        || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        playerMovement->onKeyPressed(keyCode);

        // Start rotating sprites continuously
        if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            rotateSpritesContinuously(-35.0f); // Rotate left
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            rotateSpritesContinuously(35.0f); // Rotate right
        }
    }
}

void PlayerGame3::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D
        || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        playerMovement->onKeyReleased(keyCode);

        // Stop rotating sprites when key is released
        if (!playerMovement->isMoving())
        {
            stopRotatingSprites();
        }
    }
}

void PlayerGame3::onMouseDown(Event* event)
{
    if (GameController::getInstance()->isPaused())
    {
        return;
    }

    isMouseDown = true;
}

void PlayerGame3::onMouseUp(Event* event)
{
    isMouseDown = false;
}

void PlayerGame3::shootBullet(const Vec2& target) {
    if (isMovementAndShootingDisabled) return;

    if (GameController::getInstance()->isGameOver() || GameController::getInstance()->isPaused()) return;

    Vec2 turretPosition = this->convertToWorldSpace(turretSprite->getPosition());
    if (!updateDistanceToMouse(turretPosition)) {
        return; // Do not shoot if the mouse is too close
    }

    if (timeSinceLastShot < shootDelay) return;
    if (Constants::QuantityBulletPlayerGame3 < 1) {
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::UPDATE_BULLET_LABEL, &Constants::QuantityBulletPlayerGame3);
        // Send notification to blink red
        SoundController::getInstance()->playSoundEffect(Constants::PlayerOutOfAmmoSFX);
        __NotificationCenter::getInstance()->postNotification("BlinkRedBadge", nullptr);
        return;
    }

    // Calculate the angle to the target
    Vec2 direction = target - turretPosition;

    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        direction = _touchPos - turretPosition;
    }

    float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());


    // Limit the angle within the specified range
    if (angle < minAngle) angle = minAngle;
    if (angle > maxAngle) angle = maxAngle;

    // Convert the limited angle back to a direction vector
    float limitedAngleRadians = CC_DEGREES_TO_RADIANS(angle);
    Vec2 limitedDirection = Vec2(cosf(limitedAngleRadians), sinf(limitedAngleRadians));

    // Define the angles for multiple bullets
    std::vector<float> angles;
    if (bulletCount == 1) {
        angles = { 0.0f };
    }
    else if (bulletCount == 2) {
        angles = { -7.5f, 7.5f };
    }
    else if (bulletCount == 3) {
        angles = { -15.0f, 0.0f, 15.0f };
    }

    // Shoot bullets with the calculated angles
    for (float angleOffset : angles) {
        float angleInRadians = CC_DEGREES_TO_RADIANS(angleOffset);
        Vec2 bulletDirection = Vec2(
            limitedDirection.x * cos(angleInRadians) - limitedDirection.y * sin(angleInRadians),
            limitedDirection.x * sin(angleInRadians) + limitedDirection.y * cos(angleInRadians)
        );

        auto bullet = BulletPoolPlayerGame3::getInstance()->getObject();
        if (bullet) {
            if (bullet->getParent() != nullptr) {
                bullet->removeFromParent();
            }
            bullet->setPosition(turretPosition);
            bullet->setDirection(bulletDirection);
            bullet->spawn();
            
            this->getParent()->addChild(bullet);
        }
    }

    // Update the time since the last shot and decrease the bullet count
    timeSinceLastShot = 0.0f;
    Constants::QuantityBulletPlayerGame3--;
    SoundController::getInstance()->playSoundEffect(Constants::PlayerGame3ShootSFX);
    // Send notification about bullet count change
    __NotificationCenter::getInstance()->postNotification("BulletCountChanged", nullptr);
}

void PlayerGame3::increaseBulletCount() {
    if (bulletCount < 3) {
        bulletCount++;
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

    checkCursorRange();
    updateTurretRotation();
}

void PlayerGame3::checkCursorRange() {
    Vec2 turretPositionCheck = this->convertToWorldSpace(turretSprite->getPosition());

    Vec2 direction = _mousePos - turretPositionCheck;

    // Normalize the direction vector
    direction.normalize();

    // Calculate the angle in degrees
    float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));

    if (angle < minAngle || angle > maxAngle)
    {
        updateCursorColor(true);
    }
    else
    {
        updateCursorColor(false);
    }
}

void PlayerGame3::update(float delta)
{
    if (isMovementAndShootingDisabled) return;
    playerMovement->update(delta);

    if (isMovingLeft) {
        this->setPositionX(this->getPositionX() - playerMovement->getSpeed() * delta);
    }
    if (isMovingRight) {
        this->setPositionX(this->getPositionX() + playerMovement->getSpeed() * delta);
    }

    // Get the visible size and origin of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Define padding
    float padding = SpriteController::calculateScreenRatio(0.05f); // Adjust this value as needed

    // Get the player's current position
    Vec2 currentPosition = this->getPosition();

    // Clamp the player's position within the screen boundaries with padding
    float clampedX = std::max(origin.x + padding, std::min(currentPosition.x, origin.x + visibleSize.width - padding));
    float clampedY = std::max(origin.y + padding, std::min(currentPosition.y, origin.y + visibleSize.height - padding));

    // Set the clamped position
    this->setPosition(Vec2(clampedX, clampedY));

    updateTurretRotation();

    timeSinceLastShot += delta;

    if (isMouseDown && timeSinceLastShot >= shootDelay)
    {
        shootBullet(_mousePos);
        timeSinceLastShot = 0.0f;
    }
}

void PlayerGame3::updateTurretRotation() {
    if (isMovementAndShootingDisabled) return;

    if (GameController::getInstance()->isGameOver() || GameController::getInstance()->isPaused()) return;

    if (turretSprite) {
        // Get the turret's world position
        Vec2 turretPosition = this->convertToWorldSpace(turretSprite->getPosition());

        // Calculate the direction vector from the turret to the mouse position
        Vec2 direction = _mousePos - turretPosition;

        auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
        if (platform == cocos2d::Application::Platform::OS_ANDROID ||
            platform == cocos2d::Application::Platform::OS_IPHONE) {
            direction = _touchPos - turretPosition;
        }
        // Normalize the direction vector
        direction.normalize();

        // Calculate the angle in degrees
        float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));

        if (angle < minAngle)
        {
            angle = minAngle;
        }
        else if (angle > maxAngle)
        {
            angle = maxAngle;
        }

        // Set the turret's rotation (adjusting for the initial orientation)
        turretSprite->setRotation(-angle + 90);

        // Update the distance between the player and the mouse position
        updateDistanceToMouse(turretPosition);
    }
}

float PlayerGame3::calculateDistanceToMouse(const Vec2& position) {
    return position.distance(_mousePos);
}

void PlayerGame3::updateCursorColor(bool isWithinRange) {
    auto cursor = dynamic_cast<Cursor*>(this->getParent()->getChildByName("Cursor"));
    if (cursor) {
        if (!isWithinRange) {
            cursor->setColor(Color3B::GREEN);
        }
        else {
            cursor->setColor(Color3B::RED);
        }
    }
}

bool PlayerGame3::updateDistanceToMouse(const Vec2& position) {
    distanceToMouse = calculateDistanceToMouse(position);

    // Change cursor color based on distance
    if (distanceToMouse < SpriteController::calculateScreenRatio(Constants::DISTANCE_FROM_PLAYER_TO_POINTER)) {
       updateCursorColor(true);
       return false;
    }

    return true;
}

Size PlayerGame3::GetSize() {
    return SpriteController::GetContentSize(idleSprite);
}

void PlayerGame3::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = idleSprite->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/PlayerGame3.plist", "PlayerGame3", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "PlayerGame3", originalSize, 0.3f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

void PlayerGame3::startMovingLeft() {
    isMovingLeft = true;
    isMovingRight = false;
}

void PlayerGame3::startMovingRight() {
    isMovingLeft = false;
    isMovingRight = true;
}

void PlayerGame3::stopMoving() {
    isMovingLeft = false;
    isMovingRight = false;
}
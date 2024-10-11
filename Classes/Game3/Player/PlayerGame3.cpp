// PlayerGame3.cpp
#include "PlayerGame3.h"
#include "Constants/Constants.h"
#include "Bullet/Bullet.h"
#include "Manager/PlayerMovementManager.h"
#include "cocos2d.h"

USING_NS_CC;

PlayerGame3::PlayerGame3()
    :bulletManager(nullptr),
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
    playerMovement = new PlayerMovement(this, Constants::PLAYER_SPEED_GAME3);
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
    static bool isMouseDown = false;

    if (isMouseDown) return; // Prevent multiple calls
    isMouseDown = true;

    EventMouse* e = (EventMouse*)event;
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        CCLOG("Mouse left button clicked");
        shootBullet();
    }

    // Reset the flag after a short delay
    this->scheduleOnce([&](float) { isMouseDown = false; }, 0.1f, "resetMouseDownFlag");
}

void PlayerGame3::shootBullet()
{
    CCLOG("shootBullet called");

    // Tính toán hướng từ tháp pháo đến vị trí chuột
    Vec2 direction = _mousePos - this->getPosition();
    direction.normalize();

    // Tính toán vị trí đầu của tháp pháo
    float turretAngle = CC_DEGREES_TO_RADIANS(turretSprite->getRotation() - 90); // Điều chỉnh cho góc quay của sprite
    Vec2 turretTip = turretSprite->getPosition() + Vec2(cos(turretAngle), sin(turretAngle)) * turretSprite->getContentSize().height;

    // Chuyển đổi turretTip sang tọa độ thế giới
    Vec2 worldTurretTip = turretSprite->convertToWorldSpace(Vec2(turretSprite->getContentSize().width / 2, turretSprite->getContentSize().height));

    // Tạo và khởi tạo viên đạn
    Bullet* bullet = Bullet::createBullet("assets_game/player/1.png", direction, Constants::BulletGame3Speed);

    // Kiểm tra nếu viên đạn được tạo thành công
    if (bullet)
    {
        // Đặt vị trí ban đầu của viên đạn
        bullet->setPosition(worldTurretTip);

        // Thêm viên đạn vào cảnh
        this->getParent()->addChild(bullet);

        // Thiết lập chuyển động cho viên đạn
        Vec2 velocity = direction * Constants::BulletGame3Speed;
        bullet->getPhysicsBody()->setVelocity(velocity);

        // Vô hiệu hóa vật lý cho viên đạn nếu nó có thân vật lý
        if (bullet->getPhysicsBody())
        {
            bullet->getPhysicsBody()->setEnabled(true);
        }

        // Sử dụng schedule để kiểm tra vị trí của viên đạn
        bullet->schedule([bullet](float delta) {
            auto winSize = Director::getInstance()->getWinSize();
            if (bullet->getPositionX() < 0 || bullet->getPositionX() > winSize.width ||
                bullet->getPositionY() < 0 || bullet->getPositionY() > winSize.height)
            {
                bullet->removeFromParentAndCleanup(true);
            }
            }, "check_bullet_position");
    }
    else
    {
        CCLOG("Failed to create bullet");
    }
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
    playerMovement->update(delta);

    // Update BulletManager
    bulletManager->Update(delta);

    // Update turret rotation
    updateTurretRotation(_mousePos);
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

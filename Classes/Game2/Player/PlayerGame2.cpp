// PlayerGame2.cpp
#include "PlayerGame2.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "cocos2d.h"


#include "Grenade/Grenade.h"
#include "Grenade/BulletGame2.h"
#include "Grenade/PoolBulletGame2.h"

USING_NS_CC;

PlayerGame2::PlayerGame2()
    : _mousePos(Vec2::ZERO),
    _isMouseDown(false),
    _mousePressDuration(0.0f),
    _isThrowingGrenade(false),
    playerMovement(nullptr),
    bulletPool(30),
    totalAmmo(initialAmmo),
    currentMagazine(maxMagazineSize),
    isReloading(false),
    reloadTime(2.0f),
	_health(100)
{
}

PlayerGame2::~PlayerGame2()
{
    delete playerMovement;
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
    this->setAnchorPoint(Vec2(0.5, 0.5));

    /*auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setGravityEnable(false);
    this->setPhysicsBody(physicsBody);*/

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PlayerGame2::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(PlayerGame2::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(PlayerGame2::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame2::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    this->scheduleUpdate();

    //bulletManager = new BulletManager(100, "assets_game/player/ball.png");
    playerMovement = new PlayerMovement(this, Constants::PlayerSpeed); // Properly initialize PlayerMovement
    
    _ammoLabel = Label::createWithTTF("0/0", "fonts/Marker Felt.ttf", 24);
    _ammoLabel->setPosition(Vec2(this->getContentSize().width / 2, -100)); // Đặt vị trí của label phía sau lưng người chơi
    this->addChild(_ammoLabel, 1);


    _reloadSprite = Sprite::create("assets_game/effects/Reload.png");
    _reloadSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 20));
    _reloadSprite->setVisible(false);
	_reloadSprite->setScale(0.5f);
    this->addChild(_reloadSprite, 1);
    updateAmmoDisplay();

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

    auto winSize = Director::getInstance()->getWinSize();
    _mousePos.y = winSize.height - _mousePos.y;
}

void PlayerGame2::onMouseDown(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        _isMouseDown = true;
        _mousePressDuration = 0.0f;
        _isThrowingGrenade = false;
    }
    else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        _isMouseDown = true;
        _mousePressDuration = 0.0f;
        _isThrowingGrenade = true;
    }
}

void PlayerGame2::onMouseUp(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if (_isMouseDown)
    {
        auto mousePos = Director::getInstance()->convertToGL(_mousePos);
        Vec2 pos = this->getPosition();
        Vec2 dirToShoot = mousePos - pos;

        if (_isThrowingGrenade && e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
        {
            throwGrenade(dirToShoot, _mousePressDuration);
        }
        else if (!_isThrowingGrenade && e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
        {
            shootBullet(dirToShoot);
        }

        _isMouseDown = false;
    }
}


void PlayerGame2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_A ||
        keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D ||
        keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
        keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        playerMovement->onKeyPressed(keyCode);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_R)
    {
        reload();
    }
}

void PlayerGame2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_A ||
        keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D ||
        keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
        keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        playerMovement->onKeyReleased(keyCode);
    }
}

void PlayerGame2::update(float delta)
{
    playerMovement->update(delta);
    RotateToMouse();
    if (playerMovement->getSpeed() > 0)
    {
        startMovementAnimation();
    }
    else
    {
        modelCharac->stopActionByTag(1);
    }

    if (_isMouseDown)
    {
        _mousePressDuration += delta;
    }
    if (isReloading)
    {
        reloadTime -= delta;
        if (reloadTime <= 0)
        {
            int bulletsToReload = maxMagazineSize - currentMagazine;
            if (totalAmmo >= bulletsToReload)
            {
                currentMagazine += bulletsToReload;
                totalAmmo -= bulletsToReload;
            }
            else
            {
                currentMagazine += totalAmmo;
                totalAmmo = 0;
            }
            isReloading = false;
            reloadTime = 2.0f;
            _reloadSprite->stopAllActions();
            _reloadSprite->setVisible(false);
            updateAmmoDisplay();
        }
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
    if (isReloading || currentMagazine <= 0)
    {
        return; // Cannot shoot while reloading or if the magazine is empty
    }

    Vec2 normalizedDirection = direction.getNormalized();
    BulletGame2* bullet = BulletGame2::createBullet(this->getPosition(), normalizedDirection, Constants::BulletSpeed, Constants::BulletDamage2);
    if (bullet)
    {
        this->getParent()->addChild(bullet);
    }

    currentMagazine--;
    updateAmmoDisplay(); // Update ammo display after shooting
}




void PlayerGame2::throwGrenade(const Vec2& direction, float duration)
{
    auto grenade = Grenade::createGrenade(this->getPosition(), direction, duration);
    this->getParent()->addChild(grenade);
}

void PlayerGame2::die()
{
    this->removeFromParent();
}
void PlayerGame2::reload()
{
    if (isReloading || currentMagazine == maxMagazineSize || totalAmmo == 0)
    {
        return; // Already reloading, magazine is full, or no ammo left
    }

    isReloading = true;
    reloadTime = 2.0f; // Start reload time
    _reloadSprite->setVisible(true);

    auto rotateAction = RotateBy::create(1.0f, 360.0f);
    _reloadSprite->runAction(RepeatForever::create(rotateAction));
}
void PlayerGame2::updateAmmoDisplay()
{
    if (_ammoLabel)
    {
        _ammoLabel->setString(StringUtils::format("%d/%d", currentMagazine, totalAmmo));
    }
}
void PlayerGame2::takeDamage(int damage) {
    // Implement the logic for taking damage
    // For example, reduce the player's health
    // If health reaches 0, call the die() method
    // Assuming you have a health member variable
    _health -= damage;
    if (_health <= 0) {
        die();
    }
}
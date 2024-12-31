#include "PlayerGame2.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "cocos2d.h"
#include "utils/PhysicsShapeCache.h"
#include "Controller/SoundController.h"
#include "Manager/ObjectPoolGame2.h"

USING_NS_CC;

PlayerGame2::PlayerGame2()
    : _mousePos(Vec2::ZERO),
    _isMouseDown(false),
    _mousePressDuration(0.0f),
    _isThrowingGrenade(false),
    playerMovement(nullptr),
    attributes(new PlayerAttributes(100, 130)),
    isReloading(false),
    reloadTime(2.0f),
    isMouseDown(false),
    shootDelay(0.7f),
    timeSinceLastShot(0.0f),
    bulletCount(1),
    isMovementAndShootingDisabled(false)
{
    totalAmmo = attributes->GetAmmo();
    currentMagazine = maxMagazineSize;
    currentGrenades = maxGrenades;

    SoundController::getInstance()->preloadMusic("assets_game/sounds/Game2/reload.mp3");
    SoundController::getInstance()->preloadMusic("assets_game/sounds/Game2/shot.mp3");
    SoundController::getInstance()->preloadMusic("assets_game/sounds/Game2/nembom.mp3");
}


PlayerGame2::~PlayerGame2()
{
    delete playerMovement;
	delete attributes;
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
    this->setAnchorPoint(Vec2(0.5, 0.5));

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

    playerMovement = new PlayerMovement(this, Constants::PlayerGame2Speed);
    
    _ammoLabel = Label::createWithTTF("0/0", Constants::FONT_GAME, SpriteController::calculateScreenRatio(0.01f));
    _ammoLabel->setPosition(Vec2(this->getContentSize().width / 2, -SpriteController::calculateScreenRatio(0.04f))); 
    this->addChild(_ammoLabel, 1);


    _reloadSprite = Sprite::create("assets_game/effects/Reload.png");
    _reloadSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height));
    _reloadSprite->setVisible(false);
	_reloadSprite->setScale(SpriteController::updateSpriteScale(_reloadSprite,0.05f));
    this->addChild(_reloadSprite, 1);
    this->updateAmmoDisplay();
    return true;
}

void PlayerGame2::initAnimation()
{
    // Initialize body animation
    initBodyAnimation();

    // Initialize hands animation
    initHandsAnimation();
}

void PlayerGame2::initBodyAnimation()
{
    bodySpriteBatchNode = SpriteBatchNode::create("assets_game/player/PlayerGame2Body.png");
    this->addChild(bodySpriteBatchNode);

    bodySprite = Sprite::createWithSpriteFrameName("PlayerGame2Body0.png");
    bodySprite->setScale(SpriteController::updateSpriteScale(bodySprite, 0.05f));

    bodySpriteBatchNode->addChild(bodySprite);

    auto animateBody = Animate::create(createAnimation("PlayerGame2Body", 13, 0.07f));
    bodySprite->runAction(RepeatForever::create(animateBody));
    this->createPhysicsBody();
}

void PlayerGame2::initHandsAnimation()
{
    handsSpriteBatchNode = SpriteBatchNode::create("assets_game/player/PlayerGame2Hands.png");
    this->addChild(handsSpriteBatchNode);

    handsSprite = Sprite::createWithSpriteFrameName("PlayerGame2Hands1.png");
    handsSprite->setAnchorPoint(Vec2(0.5, 0.5));
    handsSprite->setScale(SpriteController::updateSpriteScale(handsSprite, 0.01f));
    handsSprite->setPosition(Vec2(2, 0.5));

    handsSpriteBatchNode->addChild(handsSprite);

    auto animateHands = Animate::create(createAnimation("PlayerGame2Hands", 13, 0.07f));
    handsSprite->runAction(RepeatForever::create(animateHands));
}

void PlayerGame2::startMovementAnimation()
{
   /* if (!this->getActionByTag(1))
    {
        auto animateCharac = Animate::create(createAnimation("PlayerGame2Hands", 5, 0.07f));
        auto repeatAnimate = RepeatForever::create(animateCharac);
        repeatAnimate->setTag(1);
        modelCharac->runAction(repeatAnimate);
    }*/
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
        isMouseDown = true;
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
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        isMouseDown = false;
    }
    else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        _isMouseDown = false;
    }
}

void PlayerGame2::setMovementAndShootingDisabled(bool disabled)
{
    isMovementAndShootingDisabled = disabled;
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
    timeSinceLastShot += delta;

    playerMovement->update(delta);
    RotateToMouse();
    if (playerMovement->getSpeed() > 0)
    {
        startMovementAnimation();
        Vec2 velocity = playerMovement->getVelocity();
        if (velocity.x < 0) {
            bodySprite->setFlippedX(true);
            handsSprite->setFlippedX(true);
        }
        else if (velocity.x > 0) {
            bodySprite->setFlippedX(false);
            handsSprite->setFlippedX(false);
        }
    }

    if (isMouseDown) {
        auto mousePos = Director::getInstance()->convertToGL(_mousePos);
        Vec2 pos = this->getPosition();
        Vec2 dirToShoot = mousePos - pos;
        shootBullet(dirToShoot);
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
    handsSprite->setRotation(-angle);

    // Flip body and hands based on the direction
    if (dirToFace.x < 0) {
        bodySprite->setFlippedX(true);
        handsSprite->setFlippedX(true);
    }
    else {
        bodySprite->setFlippedX(false);
        handsSprite->setFlippedX(false);
    }
}

void PlayerGame2::shootBullet(const Vec2& direction)
{
    if (isMovementAndShootingDisabled) return;

    if (timeSinceLastShot < shootDelay) return;

    if (isReloading || currentMagazine <= 0)
    {
        reload();
        return;
    }

    Vec2 normalizedDirection = direction.getNormalized();
    BulletGame2* bullet = BulletGame2Pool::getInstance()->getObject();
    if (bullet) {
        bullet->setPosition(this->getPosition());
        bullet->setDirection(direction);
        bullet->setSpeed(Constants::BulletGame3Speed);
        bullet->spawn();

        if (bullet->getParent() != nullptr) {
            bullet->removeFromParent();
        }
        this->getParent()->addChild(bullet);
        currentMagazine--;
        updateAmmoDisplay();
        playShootSound();
    }

    currentMagazine--;
    updateAmmoDisplay();
    timeSinceLastShot = 0.0f;
}

void PlayerGame2::throwGrenade(const Vec2& direction, float duration)
{
    if (currentGrenades > 0 && !_isThrowingGrenade)
    {
        _isThrowingGrenade = true;
        currentGrenades--;

        auto grenade = GrenadeEnemyPool::getInstance()->getObject();
        grenade->setup(this->getPosition(), direction, duration);
        this->getParent()->addChild(grenade);

        grenade->runAction(Sequence::create(
            DelayTime::create(duration + 2.0f), // Duration of throw + explosion delay
            CallFunc::create([grenade]() {
                GrenadeEnemyPool::getInstance()->returnObject(grenade);
                }),
            nullptr
        ));

        this->playGrenadeSound();
    }
}


void PlayerGame2::reload()
{
    if (isReloading || currentMagazine == maxMagazineSize || totalAmmo == 0)
    {
        return;
    }

    isReloading = true;
    playReloadSound();
    reloadTime = 2.0f; // Start reload time
    _reloadSprite->setVisible(true);

    auto rotateAction = RotateBy::create(1.0f, 360.0f);
    _reloadSprite->runAction(RepeatForever::create(rotateAction));
    playReloadSound();
}

void PlayerGame2::takeDamage(int damage)
{
    attributes->TakeDamage(damage);
    if (attributes->IsDead())
    {
        die();
    }
}
void PlayerGame2::die()
{
    //animation cho nay
}

void PlayerGame2::pickUpHealth(int healthAmount)
{
    attributes->IncreaseHealth(healthAmount);
}
void PlayerGame2::pickUpAmmo(int ammoAmount)
{
    attributes->SetAmmo(attributes->GetAmmo() + ammoAmount);
    updateAmmoDisplay();
}
void PlayerGame2::pickUpGrenade(int grenadeAmount)
{
    currentGrenades = std::min(currentGrenades + grenadeAmount, maxGrenades);
}

void PlayerGame2::updateAmmoDisplay()
{
    _ammoLabel->setString(StringUtils::format("%d/%d", currentMagazine, totalAmmo));
}

void PlayerGame2::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = bodySprite->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/PlayerGame2.plist", "PlayerGame2", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "PlayerGame2", originalSize, 0.1f);

    if (physicsBody) {
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);
        physicsBody->setContactTestBitmask(true);
        physicsBody->setCollisionBitmask(0x02);
        this->setPhysicsBody(physicsBody);
    }
}

Size PlayerGame2::GetSize() {
    return GetContentSizeSprite(bodySprite);
}

void PlayerGame2::playReloadSound() {
    SoundController::getInstance()->playSoundEffect("assets_game/sounds/Game2/reload.mp3");
}

void PlayerGame2::playShootSound() {
    SoundController::getInstance()->playSoundEffect("assets_game/sounds/Game2/shot.mp3");
}
void PlayerGame2::playGrenadeSound() {
    SoundController::getInstance()->playSoundEffect("assets_game/sounds/Game2/nembom.mp3");
}
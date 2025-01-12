#include "Game2/Player/BulletGame2.h"
#include "utils/PhysicsShapeCache.h"
#include "Manager/ObjectPoolGame2.h"
#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "FX/Explodable.h"

USING_NS_CC;

BulletGame2* BulletGame2::create()
{
    BulletGame2* bullet = new (std::nothrow) BulletGame2();
    if (bullet && bullet->init()) {
        bullet->autorelease();
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool BulletGame2::init()
{
    if (!Sprite::init()) {
        return false;
    }

    initAnimation();

    this->setSpeed(Constants::BulletGame3Speed);

    return true;
}

void BulletGame2::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/player/BulletPlayer3Game.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode, Constants::ORDER_LAYER_PLAYER - 1);
    }

    modelCharac = Sprite::createWithSpriteFrameName("BulletPlayer3Game1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.05f)); // Adjust scale as needed
    spriteBatchNode->addChild(modelCharac, Constants::ORDER_LAYER_PLAYER - 1);

    auto animateCharac = Animate::create(SpriteController::createAnimation("BulletPlayer3Game", 31, 0.005f)); // Adjust frame count and duration as needed
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void BulletGame2::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/BulletPlayer3Game.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("BulletPlayer3Game", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "BulletPlayer3Game", originalSize, 0.1f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);
        this->setPhysicsBody(physicsBody);
    }
}

Size BulletGame2::GetSize() {
    return GetContentSize(modelCharac);
}

void BulletGame2::setDirection(const Vec2& direction)
{
    this->createPhysicsBody();
    _direction = direction;
    _direction.normalize();

    // Calculate the angle in degrees between the direction vector and the upward vector (0, 1)
    float angle = -CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x)) + 90;

    // Set the rotation of the bullet sprite
    this->setRotation(angle);

    // Schedule the update method to move the bullet
    this->spawn();
}

void BulletGame2::setSpeed(float speed)
{
    _speed = speed;
}

void BulletGame2::reset()
{
    this->setRotation(0);
    if (modelCharac) {
        modelCharac->setVisible(true);
    }
    if (explosionSprite) {
        explosionSprite->setVisible(false);
    }
    this->stopAllActions();
    this->unscheduleUpdate(); // Stop the update when resetting
    this->setVisible(true);
}

void BulletGame2::spawn()
{
    this->scheduleUpdate(); // Schedule the update method
}

void BulletGame2::hideModelCharac() {
    if (modelCharac) {
        modelCharac->setVisible(false);
    }
}

void BulletGame2::removeWhenOutOfScreen()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    Vec2 position = this->getPosition();

    if (position.x < origin.x || position.x > origin.x + visibleSize.width ||
        position.y < origin.y || position.y > origin.y + visibleSize.height)
    {
        this->returnPool();
    }
}

void BulletGame2::returnPool() {
    this->unscheduleUpdate(); // Stop the update when returning to the pool
    this->stopAllActions();
    this->reset();
    if (this->getParent() != nullptr) {
        this->removeFromParent();
    }
    // Assuming there's a BulletGame2Pool similar to SniperBulletGame2Pool
    BulletGame2Pool::getInstance()->returnObject(this);
}

void BulletGame2::explode() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    SoundController::getInstance()->playSoundEffect(Constants::EnemyCrepExplodeSFX);
    auto explosion = Explosion::create(this->getPosition(), [this]() {
        });
    if (this->getParent() != nullptr) {
        this->getParent()->addChild(explosion);
    }
    this->returnPool();
}

void BulletGame2::update(float delta)
{
    this->setPosition(this->getPosition() + _direction * _speed * delta);
    this->removeWhenOutOfScreen();
}

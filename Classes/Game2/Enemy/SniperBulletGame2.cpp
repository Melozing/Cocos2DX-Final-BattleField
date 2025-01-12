#include "SniperBulletGame2.h"
#include "Constants/Constants.h"
#include "utils/PhysicsShapeCache.h"
#include "Manager/ObjectPoolGame2.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "FX/Explodable.h"

USING_NS_CC;

SniperBulletGame2* SniperBulletGame2::create()
{
    SniperBulletGame2* bullet = new (std::nothrow) SniperBulletGame2();
    if (bullet && bullet->init()) {
        bullet->autorelease();
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool SniperBulletGame2::init()
{
    if (!Sprite::init()) {
        return false;
    }

    initAnimation();

    this->setSpeed(Constants::BulletSniperGame2Speed);

    return true;
}

void SniperBulletGame2::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/player/BulletPlayer3Game.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode, Constants::ORDER_LAYER_CHARACTER - 1);
    }

    modelCharac = Sprite::createWithSpriteFrameName("BulletPlayer3Game1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.05f)); // Adjust scale as needed
    spriteBatchNode->addChild(modelCharac, Constants::ORDER_LAYER_CHARACTER - 1);

    auto animateCharac = Animate::create(SpriteController::createAnimation("BulletPlayer3Game", 31, 0.005f)); // Adjust frame count and duration as needed
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void SniperBulletGame2::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/BulletPlayer3Game.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("BulletPlayer3Game", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "BulletPlayer3Game", originalSize, 0.02f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);
        this->setPhysicsBody(physicsBody);
    }
}

Size SniperBulletGame2::GetSize() {
    return GetContentSize(modelCharac);
}

void SniperBulletGame2::setDirection(const Vec2& direction)
{
    this->createPhysicsBody();
    _direction = direction;
    _direction.normalize();

    // Calculate the angle in degrees between the direction vector and the upward vector (0, 1)
    float angle = -CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x)) + 90;

    // Set the rotation of the bullet sprite
    this->setRotation(angle);

    // Move the bullet in the direction using MoveBy
    float distance = 1000.0f; // Distance the bullet will travel
    Vec2 targetPosition = this->getPosition() + _direction * distance;
    float duration = distance / _speed; // Calculate duration based on speed

    auto moveAction = MoveTo::create(duration, targetPosition);

    this->runAction(Sequence::create(moveAction, nullptr));
}

void SniperBulletGame2::setSpeed(float speed)
{
    _speed = speed;
}

void SniperBulletGame2::reset()
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

void SniperBulletGame2::spawn()
{
    this->scheduleUpdate(); // Schedule the update method
}

void SniperBulletGame2::hideModelCharac() {
    if (modelCharac) {
        modelCharac->setVisible(false);
    }
}

void SniperBulletGame2::removeWhenOutOfScreen()
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

void SniperBulletGame2::returnPool() {
    this->unscheduleUpdate(); // Stop the update when returning to the pool
    this->stopAllActions();
    this->reset();
    if (this->getParent() != nullptr) {
        this->removeFromParent();
    }
    SniperBulletGame2Pool::getInstance()->returnObject(this);
}

void SniperBulletGame2::explode() {
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

void SniperBulletGame2::update(float delta)
{
    this->setPosition(this->getPosition() + _direction * _speed * delta);
    this->removeWhenOutOfScreen();
}

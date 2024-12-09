#include "Bullet/BulletPlayerGame3.h"
#include "BulletPoolPlayerGame3.h"
#include "Constants/Constants.h"
#include "utils/PhysicsShapeCache.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"

USING_NS_CC;

BulletPlayerGame3* BulletPlayerGame3::create()
{
    BulletPlayerGame3* bullet = new (std::nothrow) BulletPlayerGame3();

    if (bullet && bullet->init())
    {
        bullet->autorelease();
        return bullet;
    }
    else
    {
        delete bullet;
        return nullptr;
    }
}

bool BulletPlayerGame3::init()
{
    if (!Node::init()) {
        return false;
    }

    initAnimation();

    this->setSpeed(Constants::BulletGame3Speed);
    return true;
}

void BulletPlayerGame3::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/player/BulletPlayer3Game.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName("BulletPlayer3Game1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.1f)); // Adjust scale as needed
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("BulletPlayer3Game", 31, 0.005f)); // Adjust frame count and duration as needed
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void BulletPlayerGame3::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/BulletPlayer3Game.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("BulletPlayer3Game", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "BulletPlayer3Game", originalSize, 0.5f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);
        this->setPhysicsBody(physicsBody);
    }
}

Size BulletPlayerGame3::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void BulletPlayerGame3::setDirection(const Vec2& direction)
{
    _direction = direction;
    _direction.normalize();
}

void BulletPlayerGame3::setSpeed(float speed)
{
    _speed = speed;
}

void BulletPlayerGame3::reset()
{
    this->setVisible(true);
    if (modelCharac) {
        modelCharac->setVisible(true);
    }
    if (explosionSprite) {
        explosionSprite->setVisible(false);
    }
    this->stopAllActions();
}

void BulletPlayerGame3::spawn(const Vec2& startPosition, float angle)
{
    this->setRotation(0);
    this->createPhysicsBody();
    this->setPosition(startPosition);
    this->setRotation(angle);
    this->startMovement();
}

void BulletPlayerGame3::hideModelCharac() {
    if (modelCharac) {
        modelCharac->setVisible(false);
    }
}

void BulletPlayerGame3::startMovement()
{
    // Calculate the target position based on direction and speed
    Vec2 targetPosition = this->getPosition() + _direction * _speed * 5; // Adjust the multiplier as needed

    // Create a MoveTo action
    auto moveAction = MoveTo::create(3.0f, targetPosition); // Adjust the duration as needed

    // Create a callback to remove the bullet when the action is done
    auto removeCallback = CallFunc::create([this]() {
        this->returnPool();
        });

    // Run the actions in sequence
    this->runAction(Sequence::create(moveAction, removeCallback, nullptr));
}

void BulletPlayerGame3::removeWhenOutOfScreen()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    Vec2 position = this->getPosition();

    if (position.x < origin.x || position.x > origin.x + visibleSize.width /2 ||
        position.y < origin.y || position.y > origin.y + visibleSize.height /2)
    {
        this->returnPool();
    }
}

void BulletPlayerGame3::returnPool() {
    this->removeFromParentAndCleanup(false);
    BulletPoolPlayerGame3::getInstance()->returnBullet(this);
}

void BulletPlayerGame3::explode() {
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

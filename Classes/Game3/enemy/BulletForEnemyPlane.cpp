#include "BulletForEnemyPlane.h"
#include "BulletForEnemyPlanePool.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"

USING_NS_CC;

BulletForEnemyPlane* BulletForEnemyPlane::createBullet() {
    BulletForEnemyPlane* bullet = new (std::nothrow) BulletForEnemyPlane();
    if (bullet && bullet->init()) {
        bullet->autorelease();
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool BulletForEnemyPlane::init() {
    if (!Sprite::init()) {
        return false;
    }

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");

    modelCharac = Sprite::create("assets_game/enemies/BulletPlane.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.03f));
    this->addChild(modelCharac);

    this->createPhysicsBody();

    // Initialize explosion batch node
    explosionBatchNode = SpriteBatchNode::create("assets_game/fx/explosions.png");
    this->addChild(explosionBatchNode);

    return true;
}

Size BulletForEnemyPlane::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void BulletForEnemyPlane::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsBody = PhysicsBody::createBox(this->GetSize() * 0.4f);

    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    this->setPhysicsBody(physicsBody);
}

void BulletForEnemyPlane::reset() {
    modelCharac->setVisible(true);
    this->setVisible(true);
    this->setRotation(0);
}

// BulletForEnemyPlane.cpp

void BulletForEnemyPlane::moveDown(float angle) {
    this->createPhysicsBody();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Calculate the target position within the screen bounds
    float targetX = this->getPositionX() + visibleSize.height * tan(CC_DEGREES_TO_RADIANS(angle));
    float targetY = origin.y;

    // Ensure the target position is within the screen bounds
    targetX = std::max(origin.x, std::min(targetX, origin.x + visibleSize.width));

    Vec2 targetPosition = Vec2(targetX, targetY);
    float moveDuration = 0.8f;

    // Rotate the bullet sprite to match the angle
    this->setRotation(-angle - 90); // Adjust rotation to match the sprite's initial orientation

    auto moveAction = MoveTo::create(moveDuration, targetPosition);
    auto sequence = Sequence::create(moveAction, CallFunc::create([this]() {
        this->returnToPool();
        }), nullptr);
    this->runAction(sequence);
}

void BulletForEnemyPlane::explode() {
    // Stop all actions to prevent further movement
    this->stopAllActions();

    // Create explosion effect
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    if (!explosionSprite) {
        explosionSprite = Sprite::createWithSpriteFrameName("explosions7.png");
        explosionSprite->setScale(SpriteController::updateSpriteScale(explosionSprite, 0.078f));
        explosionBatchNode->addChild(explosionSprite);
    }

    explosionSprite->setPosition(modelCharac->getPosition());
    modelCharac->setVisible(false);
    explosionSprite->setVisible(true);

    auto explosionAnimation = SpriteController::createAnimation("explosions", 10, 0.041f);
    auto animate = Animate::create(explosionAnimation);

    explosionSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([this]() {
            explosionSprite->setVisible(false);
            this->returnToPool();
            }),
        nullptr
    ));
}

void BulletForEnemyPlane::returnToPool() {
    this->stopAllActions();
    this->removeFromParent();
    this->setVisible(false);
    BulletForEnemyPlanePool::getInstance()->returnBullet(this);
}
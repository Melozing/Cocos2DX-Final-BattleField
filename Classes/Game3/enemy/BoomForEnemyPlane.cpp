#include "BoomForEnemyPlane.h"
#include "BoomForEnemyPlanePool.h"
#include "utils/PhysicsShapeCache.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"

USING_NS_CC;

BoomForEnemyPlane* BoomForEnemyPlane::createBoom() {
    BoomForEnemyPlane* boom = new (std::nothrow) BoomForEnemyPlane();
    if (boom && boom->init()) {
        boom->autorelease();
        return boom;
    }
    CC_SAFE_DELETE(boom);
    return nullptr;
}

bool BoomForEnemyPlane::init() {
    if (!Sprite::init()) {
        return false;
    }

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");

    modelCharac = Sprite::create("assets_game/enemies/Boom.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    this->addChild(modelCharac);

    this->createPhysicsBody();

    // Initialize explosion batch node
    explosionBatchNode = SpriteBatchNode::create("assets_game/fx/explosions.png");
    this->addChild(explosionBatchNode);

    return true;
}

Size BoomForEnemyPlane::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void BoomForEnemyPlane::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/BombForEnemyPlane2.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("BombForEnemyPlane2", originalSize, scaledSize);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

void BoomForEnemyPlane::reset() {
    modelCharac->setVisible(true);
    this->setVisible(true);
    this->setRotation(0);
}

void BoomForEnemyPlane::moveDown(bool spawnFromLeft) {
    this->createPhysicsBody();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float moveDuration = 4.0f;
    float rotateDuration = 2.0f;
    Vec2 moveBy = Vec2(0, -visibleSize.height);

    // Adjust the movement to be diagonal based on the spawn direction
    float horizontalShift = visibleSize.width / 10; // Reduce the horizontal shift

    float rotationAngle = 15.0f; // Define the rotation angle

    if (!spawnFromLeft) {
        moveBy.x = horizontalShift; // Move to the right
        rotationAngle = -rotationAngle; // Rotate to the left
    }
    else {
        moveBy.x = -horizontalShift; // Move to the left
        rotationAngle = rotationAngle; // Rotate to the right
    }

    auto moveAction = MoveBy::create(moveDuration, moveBy);
    auto rotateAction = RotateBy::create(rotateDuration, rotationAngle);
    auto spawnAction = Spawn::createWithTwoActions(moveAction, rotateAction);
    auto sequence = Sequence::create(spawnAction, CallFunc::create([this]() {
        this->returnToPool();
        }), nullptr);
    this->runAction(sequence);
}

void BoomForEnemyPlane::explode() {
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


void BoomForEnemyPlane::returnToPool() {
    this->stopAllActions();
    this->removeFromParent();
    this->setVisible(false);
    BoomForEnemyPlanePool::getInstance()->returnBoom(this);
}
#include "Game3/enemy/FinisherMissiles.h"
#include "Manager/ObjectPoolGame3.h"
#include "utils/PhysicsShapeCache.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"
#include "Game3/Game3Scene.h"
#include "FX/Explodable.h"
#include "cocos2d.h"

USING_NS_CC;

FinisherMissiles* FinisherMissiles::create() {
    FinisherMissiles* missile = new (std::nothrow) FinisherMissiles();
    if (missile && missile->init()) {
        missile->autorelease();
        return missile;
    }
    CC_SAFE_DELETE(missile);
    return nullptr;
}

bool FinisherMissiles::init() {
    if (!Sprite::init()) {
        return false;
    }

    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/rocket.png");
    this->addChild(spriteBatchNode);

    modelCharac = Sprite::createWithSpriteFrameName("rocket1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.15f));
    spriteBatchNode->addChild(modelCharac);
    this->createPhysicsBody();
    this->initAnimation();

    return true;
}

void FinisherMissiles::initAnimation() {
    auto animateCharac = Animate::create(SpriteController::createAnimation("rocket", 11, 0.017f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

Size FinisherMissiles::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void FinisherMissiles::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/MissileForEnemyPlane.plist", "MissileForEnemyPlane", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "MissileForEnemyPlane", originalSize, 0.30f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

void FinisherMissiles::reset() {
    modelCharac->setVisible(true);
    this->setVisible(true);
    this->setRotation(0);
}

void FinisherMissiles::moveDown(bool moveLeft) {
    this->createPhysicsBody();

    // Define the duration of the movement
    float moveDuration = 0.5f; // Adjust duration to control speed
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Determine the direction to move down based on the moveLeft parameter
    Vec2 moveDownBy;
    float rotateAngle;
    if (moveLeft) {
        // Move down to the left
        moveDownBy = Vec2(-visibleSize.width / 4, -visibleSize.height);
        rotateAngle = 45.0f; // Rotate to match the downward left movement
    }
    else {
        // Move down to the right
        moveDownBy = Vec2(visibleSize.width / 4, -visibleSize.height);
        rotateAngle = -45.0f; // Rotate to match the downward right movement
    }

    // Ensure the missile does not move outside the screen
    Vec2 targetPosition = this->getPosition() + moveDownBy;
    if (targetPosition.x < origin.x) {
        moveDownBy.x = origin.x - this->getPosition().x;
    }
    else if (targetPosition.x > origin.x + visibleSize.width) {
        moveDownBy.x = (origin.x + visibleSize.width) - this->getPosition().x;
    }

    // Move the sprite downward with a diagonal component
    auto moveDownAction = MoveBy::create(moveDuration, moveDownBy);
    auto rotateAction = RotateTo::create(0.1f, rotateAngle); // Adjust rotation to match downward movement
    auto moveWithRotation = Spawn::create(moveDownAction, rotateAction, nullptr);

    // Run the move action
    this->runAction(moveWithRotation);
}

void FinisherMissiles::explode() {
    // Stop all actions to prevent further movement
    this->stopAllActions();

    // Remove physics body if it exists
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    modelCharac->setVisible(false);
    SoundController::getInstance()->playSoundEffect(Constants::CityDamagedSFX);
    // Create explosion effect
    auto explosion = Explosion::create(this->getPosition(), [this]() {
        this->returnToPool();
        }); // Adjust the scale as needed
    this->getParent()->addChild(explosion);
}

void FinisherMissiles::returnToPool() {
    this->stopAllActions();
    this->removeFromParent();
    this->setVisible(false);
    FinisherMissilesPool::getInstance()->returnObject(this);
}

#include "MissileForEnemyPlane.h"
#include "Manager/ObjectPoolGame3.h"
#include "utils/PhysicsShapeCache.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"
#include "Game3/Game3Scene.h"
#include "FX/Explodable.h"
#include "cocos2d.h"

USING_NS_CC;

MissileForEnemyPlane* MissileForEnemyPlane::create() {
    MissileForEnemyPlane* missile = new (std::nothrow) MissileForEnemyPlane();
    if (missile && missile->init()) {
        missile->autorelease();
        return missile;
    }
    CC_SAFE_DELETE(missile);
    return nullptr;
}

bool MissileForEnemyPlane::init() {
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

void MissileForEnemyPlane::initAnimation() {
    auto animateCharac = Animate::create(SpriteController::createAnimation("rocket", 11, 0.017f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

Size MissileForEnemyPlane::GetSize() {
    return SpriteController::GetContentSize(modelCharac);
}

void MissileForEnemyPlane::createPhysicsBody() {
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

void MissileForEnemyPlane::reset() {
    modelCharac->setVisible(true);
    this->setVisible(true);
    this->setRotation(0);
}

void MissileForEnemyPlane::moveDown(bool moveLeft) {
    this->createPhysicsBody();

    // Define the radius of the circle and the duration of the movement
    float radius = 150.0f;
    float duration = 0.7f; // Reduce duration to increase speed
    float moveDuration = 3.0f; // Reduce move duration to increase speed
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Get the position and size of the CityCollisionArea
    auto cityCollisionArea = dynamic_cast<Game3Scene*>(this->getParent())->getCityCollisionArea();
    auto cityPosition = cityCollisionArea->getPosition();
    auto citySize = cityCollisionArea->getContentSize();

    // Determine the direction to move up based on the moveLeft parameter
    Vec2 moveUpBy;
    float rotateUpAngle;
    if (moveLeft) {
        // Move up to the left
        moveUpBy = Vec2(-visibleSize.width / 4, visibleSize.height / 4);
        rotateUpAngle = -360; // Rotate counterclockwise
    }
    else {
        // Move up to the right
        moveUpBy = Vec2(visibleSize.width / 4, visibleSize.height / 4);
        rotateUpAngle = 360; // Rotate clockwise
    }

    // Ensure the missile does not move outside the screen
    Vec2 targetPosition = this->getPosition() + moveUpBy;
    if (targetPosition.x < origin.x) {
        moveUpBy.x = origin.x - this->getPosition().x;
    }
    else if (targetPosition.x > origin.x + visibleSize.width) {
        moveUpBy.x = (origin.x + visibleSize.width) - this->getPosition().x;
    }

    // Create a move up action with rotation
    auto moveUpAction = MoveBy::create(duration, moveUpBy);
    auto rotateUpAction = RotateBy::create(duration, rotateUpAngle);
    auto moveUpWithRotation = Spawn::create(moveUpAction, rotateUpAction, nullptr);

    // Determine the direction to move down based on the initial move up direction
    Vec2 moveDownBy;
    float rotateDownAngle;
    if (moveLeft) {
        // Move down to the left
        moveDownBy = Vec2(visibleSize.width / 4, -visibleSize.height);
        rotateDownAngle = -45.0f; // Rotate to match the downward left movement
    }
    else {
        // Move down to the right
        moveDownBy = Vec2(-visibleSize.width / 4, -visibleSize.height);
        rotateDownAngle = 45.0f; // Rotate to match the downward right movement
    }

    // Ensure the missile stays within the CityCollisionArea
    float minX = cityPosition.x - citySize.width / 2;
    float maxX = cityPosition.x + citySize.width / 2;
    float minY = cityPosition.y - citySize.height / 2;
    float maxY = cityPosition.y + citySize.height / 2;

    // Adjust the moveDownBy vector to ensure the missile stays within the CityCollisionArea
    if (this->getPositionX() + moveDownBy.x < minX) {
        moveDownBy.x = minX - this->getPositionX();
    }
    else if (this->getPositionX() + moveDownBy.x > maxX) {
        moveDownBy.x = maxX - this->getPositionX();
    }

    if (this->getPositionY() + moveDownBy.y < minY) {
        moveDownBy.y = minY - this->getPositionY() * 2;
    }
    else if (this->getPositionY() + moveDownBy.y > maxY) {
        moveDownBy.y = maxY - this->getPositionY() * 2;
    }

    // Move the sprite downward with a random horizontal component
    auto moveDownAction = MoveBy::create(moveDuration, moveDownBy);
    auto rotateDownAction = RotateTo::create(0.1f, rotateDownAngle); // Adjust rotation to match downward movement
    auto moveDownWithRotation = Spawn::create(moveDownAction, rotateDownAction, nullptr);

    // Create a sequence of actions: move up with rotation, then move down with rotation
    auto sequence = Sequence::create(moveUpWithRotation, moveDownWithRotation, nullptr);

    // Run the sequence of actions
    this->runAction(sequence);
}

void MissileForEnemyPlane::explode() {
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

void MissileForEnemyPlane::returnToPool() {
    this->stopAllActions();
    this->removeFromParent();
    this->setVisible(false);
    MissileForEnemyPlanePool::getInstance()->returnObject(this);
}

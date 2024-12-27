#include "SuicideBomberEnemy.h"
#include "utils/PhysicsShapeCache.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

SuicideBomberEnemy* SuicideBomberEnemy::create() {
    SuicideBomberEnemy* enemy = new (std::nothrow) SuicideBomberEnemy();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    delete enemy;
    return nullptr;
}

bool SuicideBomberEnemy::init() {
    if (!EnemyBase::init()) {
        return false;
    }

    initAnimation();
    this->scheduleUpdate(); // Schedule the update method
    return true;
}

void SuicideBomberEnemy::reset() {
    EnemyBase::reset();
    // Additional reset logic for SuicideBomberEnemy if needed
}

void SuicideBomberEnemy::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/SuicideBomberEnemy.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName("SuicideBomberEnemy1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("SuicideBomberEnemy", 6, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void SuicideBomberEnemy::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/SuicideBomberEnemy.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = modelCharac->getContentSize() * modelCharac->getScale();

    auto physicsBody = physicsCache->createBody("SuicideBomberEnemy", originalSize, scaledSize);
    //physicsCache->resizeBody(physicsBody, "SuicideBomberEnemy", originalSize, 0.2f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

void SuicideBomberEnemy::setTarget(Cannon* target) {
    this->targetCannon = target;
    this->createPhysicsBody();
    setInitialDirection();
}

void SuicideBomberEnemy::setInitialDirection() {
    if (targetCannon) {
        Vec2 currentPosition = this->getPosition();
        Vec2 targetPosition = targetCannon->getPosition();
        Vec2 direction = targetPosition - currentPosition;

        flipSpriteBasedOnDirection(direction); // Use the new method
    }
}

void SuicideBomberEnemy::update(float delta) {
    if (targetCannon) {
        Vec2 currentPosition = this->getPosition();
        Vec2 targetPosition = targetCannon->getPosition();
        Vec2 direction = targetPosition - currentPosition;
        direction.normalize();
        float speed = 100.0f;
        Vec2 newPosition = currentPosition + direction * speed * delta;
        this->setPosition(newPosition);

        flipSpriteBasedOnDirection(direction); // Use the new method
    }
}

void SuicideBomberEnemy::flipSpriteBasedOnDirection(const Vec2& direction) {
    if (direction.x < 0) {
        modelCharac->setFlippedX(false);
    }
    else {
        modelCharac->setFlippedX(true);
    }
}

Size SuicideBomberEnemy::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}
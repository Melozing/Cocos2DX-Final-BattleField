#include "EnemyPlaneBoss.h"
#include "EnemyPlaneBossPool.h"
#include "Constants/Constants.h"
#include "utils/PhysicsShapeCache.h"
#include <Controller/SpriteController.h>

USING_NS_CC;

EnemyPlaneBoss* EnemyPlaneBoss::createEnemyPlaneBoss() {
    EnemyPlaneBoss* enemy = new (std::nothrow) EnemyPlaneBoss();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlaneBoss::init() {
    if (!EnemyPlaneBase::init()) {
        return false;
    }

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/B52.plist");

    initAnimation();

    // Initialize health
    health = Constants::HealthEnemyPlaneBoss;

    return true;
}

void EnemyPlaneBoss::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/B52.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName("B52.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.5f)); // Adjust scale as needed
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("B52", 1, 0.1f)); // Adjust frame count and duration as needed
    modelCharac->runAction(RepeatForever::create(animateCharac));
    createPhysicsBody();
}

void EnemyPlaneBoss::spawnEnemy(cocos2d::Node* parent) {
    auto enemy = EnemyPlaneBossPool::getInstance()->getEnemy();
    if (enemy) {
        enemy->resetSprite();
        parent->addChild(enemy);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // Set initial position to the left side of the screen
        enemy->setPosition(Vec2(origin.x - enemy->getContentSize().width / 2, origin.y + visibleSize.height / 2));

        // Create a move action to move the enemy to the center of the screen
        auto moveToCenter = MoveTo::create(2.0f, Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2)); // Adjust duration as needed

        // Run the move action
        enemy->runAction(moveToCenter);
    }
}

void EnemyPlaneBoss::reset() {
    this->stopAllActions();
    this->setVisible(true);
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/B52.png");
}

Size EnemyPlaneBoss::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void EnemyPlaneBoss::takeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        // Move the enemy off the screen to the right
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();
        auto moveOffScreen = MoveTo::create(2.0f, Vec2(origin.x + visibleSize.width + this->getContentSize().width / 2, this->getPositionY())); // Adjust duration as needed

        // Run the move action and then remove the enemy from the parent
        auto sequence = Sequence::create(moveOffScreen, CallFunc::create([this]() {
            this->removeFromParent();
            }), nullptr);

        this->runAction(sequence);
    }
}

void EnemyPlaneBoss::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/EnemyPlaneBoss.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("EnemyPlaneBoss", originalSize, scaledSize);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

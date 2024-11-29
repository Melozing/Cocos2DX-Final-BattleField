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

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/EnemyPlaneBoss.plist");

    initAnimation();

    // Initialize health
    health = Constants::HealthEnemyPlaneBoss;

    // Initialize visibleSize and origin
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    return true;
}

float EnemyPlaneBoss::getHealth() const {
    return health;
}

void EnemyPlaneBoss::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/EnemyPlaneBoss.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode, Constants::ORDER_LAYER_CHARACTER);
    }

    modelCharac = Sprite::createWithSpriteFrameName("EnemyPlaneBoss1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.7f)); // Adjust scale as needed
    spriteBatchNode->addChild(modelCharac, Constants::ORDER_LAYER_CHARACTER);

    auto animateCharac = Animate::create(SpriteController::createAnimation("EnemyPlaneBoss", 3, 0.1f)); // Adjust frame count and duration as needed
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBoss::spawnEnemy(cocos2d::Node* parent) {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    this->reset(); // Reset the boss state

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Set initial position to the top of the screen
    this->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height + this->getContentSize().height / 2));

    // Set initial scale to a smaller value
    this->setScale(0.1f);

    // Create a move action to move the enemy to the upper region of the screen
    auto moveToUpperRegion = MoveTo::create(2.0f, Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.8f)); // Adjust duration and target position as needed

    // Create a scale action to scale the enemy to its original size
    auto scaleToOriginalSize = ScaleTo::create(4.0f, 1.0f); // Adjust duration as needed

    // Create a spawn action to run both move and scale actions simultaneously
    auto spawnAction = Spawn::create(moveToUpperRegion, scaleToOriginalSize, nullptr);

    // Run the spawn action and then start moving left and right
    this->runAction(Sequence::create(spawnAction, CallFunc::create([this]() {
        this->createPhysicsBody();
        this->moveLeftRight();
        }), nullptr));
}

void EnemyPlaneBoss::moveLeftRight() {
    // Define the left and right positions
    auto moveLeft = MoveTo::create(3.0f, Vec2(origin.x + visibleSize.width * 0.35f, this->getPositionY()));
    auto moveRight = MoveTo::create(3.0f, Vec2(origin.x + visibleSize.width * 0.65f, this->getPositionY()));

    // Create a sequence to move left and right repeatedly
    auto moveSequence = Sequence::create(moveRight, moveLeft, nullptr);
    auto repeatMove = RepeatForever::create(moveSequence);

    // Run the repeat move action
    this->runAction(repeatMove);
}

void EnemyPlaneBoss::reset() {
    this->stopAllActions();
    this->setVisible(true);
    health = Constants::HealthEnemyPlaneBoss;
}

Size EnemyPlaneBoss::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void EnemyPlaneBoss::takeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        moveUpAndReturnToPool();
    }
}

void EnemyPlaneBoss::moveUpAndReturnToPool() {
    // Stop all actions to prevent further movement
    this->stopAllActions();
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    // Create a move action to move the boss to the center of the screen on the X axis
    auto moveToCenterX = MoveTo::create(1.0f, Vec2(origin.x + visibleSize.width / 2, this->getPositionY()));

    // Create a move action to move the boss up and out of the screen (higher than before)
    auto moveToTop = MoveTo::create(3.0f, Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height + this->GetSize().height * 2));

    // Create a scale action to scale the boss down to a smaller size
    auto scaleDown = ScaleTo::create(1.5f, 0.1f);

    // Create a spawn action to run both move and scale actions simultaneously
    auto moveAndScale = Spawn::create(scaleDown, moveToTop, nullptr);

    // Create a sequence to move to center X, then run the move and scale actions, then return to pool
    auto sequence = Sequence::create(moveToCenterX, moveAndScale, CallFunc::create([this]() {
        this->isExploding = false; // Stop explosions
        this->removeFromParentAndCleanup(false);
        EnemyPlaneBossPool::getInstance()->returnEnemy(this);
        }), nullptr);

    // Run the sequence action
    this->runAction(sequence);

    // Start explosions
    this->isExploding = true;
    this->startExplosions();
}

void EnemyPlaneBoss::startExplosions() {
    float delayBetweenExplosions = 0.1f; // Adjust the delay between explosions as needed

    this->schedule([this](float dt) {
        if (this->isExploding) {
            float randomX = random(-this->GetSize().width / 2, this->GetSize().width / 2);
            float randomY = random(-this->GetSize().height / 2.7, this->GetSize().height / 2.7);
            auto explosion = Explosion::create(Vec2(randomX, randomY), nullptr); // Adjust scale as needed
            this->addChild(explosion, Constants::ORDER_LAYER_CHARACTER + 1);
        }
        else {
            this->unschedule("explosion_schedule_key");
        }
        }, delayBetweenExplosions, "explosion_schedule_key");
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
    physicsCache->resizeBody(physicsBody, "EnemyPlaneBoss", originalSize, 2.40f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

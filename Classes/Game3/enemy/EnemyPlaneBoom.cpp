#include "EnemyPlaneBoom.h"
#include "Manager/ObjectPoolGame3.h"
#include "Constants/Constants.h"
#include "utils/PhysicsShapeCache.h"

USING_NS_CC;

EnemyPlaneBoom* EnemyPlaneBoom::create() {
    EnemyPlaneBoom* enemy = new (std::nothrow) EnemyPlaneBoom();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlaneBoom::init() {
    if (!EnemyPlaneBase::init()) {
        return false;
    }

    initAnimation();
    reset();
    return true;
}

Size EnemyPlaneBoom::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void EnemyPlaneBoom::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/EnemyPlaneBoom.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName("EnemyPlaneBoom1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.1f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("EnemyPlaneBoom", 39, 0.033f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBoom::spawnEnemy(cocos2d::Node* parent, float skillTime, bool spawnWithSkill, const std::string& direction, const std::string& position) {
        this->setVisible(true);
        this->resetSprite();
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // Define fixed y-coordinates for spawning
        float lowY = visibleSize.height * 0.85f;
        float midY = visibleSize.height * 0.9f;
        float highY = visibleSize.height * 0.95f;

        float fixedY;
        if (position == "low") {
            fixedY = lowY;
        }
        else if (position == "mid") {
            fixedY = midY;
        }
        else {
            fixedY = highY;
        }

        if (direction == "Right") {
            this->setPosition(Vec2(-this->getContentSize().width / 2, fixedY));
            this->moveFromLeftToRight(visibleSize, Constants::EnemyPlaneBoomGame3Speed);
        }
        else {
            this->setPosition(Vec2(visibleSize.width + this->getContentSize().width / 2, fixedY));
            this->moveFromRightToLeft(visibleSize, Constants::EnemyPlaneBoomGame3Speed);
        }

        // Create physics body when spawning
        this->createPhysicsBody();

        // Schedule to spawn boom at the specified skill time
        bool spawnFromLeft = (direction == "leftToRight");
        this->scheduleOnce([this, spawnFromLeft](float dt) {
            this->spawnBoom(spawnFromLeft);
            }, skillTime, "spawn_boom_key");
}


void EnemyPlaneBoom::spawnBoom(bool spawnFromLeft) {
    auto boom = BoomForEnemyPlanePool::getInstance()->getObject();
    if (boom) {
        boom->setPosition(this->getPosition());
        if (boom->getParent() == nullptr) {
            this->getParent()->addChild(boom);
        }
        boom->setVisible(true);
        boom->moveDown(spawnFromLeft);
    }
}

void EnemyPlaneBoom::returnToPool() {
    this->stopAllActions();
    this->unschedule("spawn_boom_key");
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);
    this->reset();
    EnemyPlaneBoomPool::getInstance()->returnObject(this);
}

void EnemyPlaneBoom::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/EnemyPlaneBoom.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("EnemyPlaneBoom", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "EnemyPlaneBoom", originalSize, 0.9f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

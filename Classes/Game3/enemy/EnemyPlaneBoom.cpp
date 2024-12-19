#include "EnemyPlaneBoom.h"
#include "Manager/ObjectPoolGame3.h"
#include "Constants/Constants.h"

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
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/enemy_plane_boom.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName("Plane_enemy_bom1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("Plane_enemy_bom", 8, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBoom::spawnEnemy(cocos2d::Node* parent, float skillTime, bool spawnWithSkill, const std::string& direction, const std::string& position) {
    auto enemy = EnemyPlaneBoomPool::getInstance()->getObject();
    if (enemy) {
        enemy->setVisible(true);
        enemy->resetSprite();
        parent->addChild(enemy);
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // Define fixed y-coordinates for spawning
        float lowY = visibleSize.height * 0.7f;
        float midY = visibleSize.height * 0.8f;
        float highY = visibleSize.height * 0.9f;

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
            enemy->setPosition(Vec2(-enemy->getContentSize().width / 2, fixedY));
            enemy->moveFromLeftToRight(visibleSize, Constants::EnemyPlaneBoomGame3Speed);
        }
        else {
            enemy->setPosition(Vec2(visibleSize.width + enemy->getContentSize().width / 2, fixedY));
            enemy->moveFromRightToLeft(visibleSize, Constants::EnemyPlaneBoomGame3Speed);
        }

        // Create physics body when spawning
        enemy->createPhysicsBody();

        // Schedule to spawn boom at the specified skill time
        enemy->scheduleOnce([enemy, spawnFromLeft = (direction == "leftToRight")](float dt) {
            enemy->spawnBoom(spawnFromLeft);
            }, skillTime, "spawn_boom_key");
    }
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

    auto physicsBody = PhysicsBody::createBox(this->GetSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    this->addComponent(physicsBody);
}

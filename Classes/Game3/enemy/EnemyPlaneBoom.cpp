#include "EnemyPlaneBoom.h"
#include "EnemyPlaneBoomPool.h"
#include "Constants/Constants.h"
#include "BoomForEnemyPlanePool.h"

USING_NS_CC;

EnemyPlaneBoom* EnemyPlaneBoom::createEnemyPlaneBoom() {
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

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/enemy_plane_boom.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");

    initAnimation();

    // Initialize explosion batch node
    explosionBatchNode = SpriteBatchNode::create("assets_game/fx/explosions.png");
    this->addChild(explosionBatchNode);

    // Initialize boom
    boom = BoomForEnemyPlane::createBoom();
    this->addChild(boom);

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

void EnemyPlaneBoom::spawnEnemy(cocos2d::Node* parent) {
    auto enemy = EnemyPlaneBoomPool::getInstance()->getEnemy();
    if (enemy) {
        enemy->resetSprite();
        parent->addChild(enemy);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float randomY = random(visibleSize.height / 2, visibleSize.height);
        bool spawnFromLeft = random(0, 1) == 0;

        if (spawnFromLeft) {
            enemy->setPosition(Vec2(-enemy->getContentSize().width / 2, randomY));
            enemy->moveFromLeftToRight(visibleSize, Constants::EnemyPlaneBoomGame3Speed);
        }
        else {
            enemy->setPosition(Vec2(visibleSize.width + enemy->getContentSize().width / 2, randomY));
            enemy->moveFromRightToLeft(visibleSize, Constants::EnemyPlaneBoomGame3Speed);
        }

        // Schedule to spawn boom after 2 seconds
        enemy->scheduleOnce(CC_SCHEDULE_SELECTOR(EnemyPlaneBoom::spawnBoom), 2.0f);
    }
}

void EnemyPlaneBoom::spawnBoom(float dt) {
    if (boom) {
        boom->setPosition(this->getPosition());
        boom->setVisible(true);
        boom->moveDown();
    }
}

void EnemyPlaneBoom::reset() {
    // Reset the state of the EnemyPlaneBoom
    this->setVisible(true);
    this->setPosition(Vec2::ZERO);
    this->stopAllActions();
    if (boom) {
        boom->reset();
    }
}

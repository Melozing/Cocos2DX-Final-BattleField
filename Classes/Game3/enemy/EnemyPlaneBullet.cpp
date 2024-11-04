#include "EnemyPlaneBullet.h"
#include "EnemyPlaneBulletPool.h"
#include "Constants/Constants.h"

USING_NS_CC;

EnemyPlaneBullet* EnemyPlaneBullet::createEnemyBullet() {
    EnemyPlaneBullet* enemy = new (std::nothrow) EnemyPlaneBullet();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlaneBullet::init() {
    if (!EnemyPlaneBase::init()) {
        return false;
    }

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/enemy_plane_boom.plist");

    initAnimation(); // Ensure animation is initialized when the enemy is created

    return true;
}

void EnemyPlaneBullet::initAnimation() {
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

void EnemyPlaneBullet::spawnEnemy(cocos2d::Node* parent) {
    auto enemy = EnemyPlaneBulletPool::getInstance()->getEnemy();
    if (enemy) {
        enemy->resetSprite(); 
        parent->addChild(enemy);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float randomY = random(visibleSize.height / 2, visibleSize.height);
        bool spawnFromLeft = random(0, 1) == 0;

        if (spawnFromLeft) {
            enemy->setPosition(Vec2(-enemy->getContentSize().width / 2, randomY));
            enemy->moveFromLeftToRight(visibleSize, Constants::EnemyGame3Speed_1);
        }
        else {
            enemy->setPosition(Vec2(visibleSize.width + enemy->getContentSize().width / 2, randomY));
            enemy->moveFromRightToLeft(visibleSize, Constants::EnemyGame3Speed_1);
        }
    }
}

void EnemyPlaneBullet::reset() {
    this->stopAllActions();
    this->setVisible(true);
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/enemy_plane_boom.png");
}

#include "EnemyPlaneBoss.h"
#include "EnemyPlaneBossPool.h"
#include "Constants/Constants.h"
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

        /*enemy->resetSprite();
        parent->addChild(enemy);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float randomY = random(visibleSize.height / 2, visibleSize.height);
        bool spawnFromLeft = random(0, 1) == 0;*/

        //if (spawnFromLeft) {
        //    enemy->setPosition(Vec2(-enemy->getContentSize().width / 2, randomY));
        //    enemy->moveFromLeftToRight(visibleSize, Constants::SpeedEnemyPlane_BOSS); // Adjust speed constant as needed
        //}
        //else {
        //    enemy->setPosition(Vec2(visibleSize.width + enemy->getContentSize().width / 2, randomY));
        //    enemy->moveFromRightToLeft(visibleSize, Constants::SpeedEnemyPlane_BOSS); // Adjust speed constant as needed
        //}
    }
}


void EnemyPlaneBoss::reset() {
    this->stopAllActions();
    this->setVisible(true);
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/B52.png");
}

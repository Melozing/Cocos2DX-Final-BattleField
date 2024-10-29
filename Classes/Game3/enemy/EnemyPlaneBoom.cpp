#include "EnemyPlaneBoom.h"
#include "Constants/Constants.h"

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

    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/enemy_plane_boom.png");
    this->addChild(spriteBatchNode);

    initAnimation();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float randomY = random(visibleSize.height / 2, visibleSize.height);
    this->setPosition(Vec2(-this->getContentSize().width / 2, randomY));
    this->moveAndReturn(visibleSize, Constants::SpeedEnemyPlaneBoom);


    return true;
}

void EnemyPlaneBoom::initAnimation() {
    modelCharac = Sprite::createWithSpriteFrameName("Plane_enemy_bom1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac,0.07f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("Plane_enemy_bom", 8, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBoom::spawnEnemyAfterDelay(float delay, Node* parent) {
    EnemyPlaneBase::spawnEnemyAfterDelay(delay, parent, []() {
        return EnemyPlaneBoom::createEnemyPlaneBoom();
        });
}

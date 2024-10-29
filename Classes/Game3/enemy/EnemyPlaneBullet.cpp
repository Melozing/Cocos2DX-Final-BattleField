#include "EnemyPlaneBullet.h"
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

    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/enemy_plane_boom.png");
    this->addChild(spriteBatchNode);
  
    initAnimation();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float randomY = random(visibleSize.height / 2, visibleSize.height);
    this->setPosition(Vec2(-this->getContentSize().width / 2, randomY));
    this->moveAndReturn(visibleSize, Constants::EnemyGame3Speed_1);


    return true;
}

void EnemyPlaneBullet::initAnimation() {
    modelCharac = Sprite::createWithSpriteFrameName("Plane_enemy_bom1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("Plane_enemy_bom", 8, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBullet::spawnEnemyAfterDelay(float delay, Node* parent) {
    EnemyPlaneBase::spawnEnemyAfterDelay(delay, parent, []() {
        return EnemyPlaneBullet::createEnemyBullet();
        });
}

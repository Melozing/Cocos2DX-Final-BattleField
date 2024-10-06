#include "EnemyPlan3.h"
#include "cocos2d.h"
#include "Constants/Constants.h"

USING_NS_CC;

EnemyPlan3* EnemyPlan3::createEnemyPlan3() {
    EnemyPlan3* enemy = new (std::nothrow) EnemyPlan3();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlan3::init() {
    
    if (!Sprite::initWithFile("assets_game/enemies/Ship.png")) {
        return false;
    }

    
    auto visibleSize = Director::getInstance()->getVisibleSize();

   
    this->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - this->getContentSize().height / 2));

    this->setScale(Constants::PlayerScale3);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    return true;
}

void EnemyPlan3::move() {
    
}

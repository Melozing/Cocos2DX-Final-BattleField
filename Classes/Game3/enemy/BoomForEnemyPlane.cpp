#include "BoomForEnemyPlane.h"

USING_NS_CC;

BoomForEnemyPlane* BoomForEnemyPlane::createBoom() {
    BoomForEnemyPlane* boom = new (std::nothrow) BoomForEnemyPlane();
    if (boom && boom->init()) {
        boom->autorelease();
        return boom;
    }
    CC_SAFE_DELETE(boom);
    return nullptr;
}

bool BoomForEnemyPlane::init() {
    if (!Sprite::init()) {
        return false;
    }
    this->initWithFile("assets_game/player/1.png");
    return true;
}

void BoomForEnemyPlane::reset() {
    this->setVisible(true);
}

void BoomForEnemyPlane::moveDown() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto moveAction = MoveTo::create(2.0f, Vec2(this->getPositionX(), -this->getContentSize().height / 2));
    this->runAction(moveAction);
}

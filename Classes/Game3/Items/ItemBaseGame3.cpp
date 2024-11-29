#include "Game3/Items/ItemBaseGame3.h"
USING_NS_CC;

bool ItemBaseGame3::init() {
    if (!Sprite::init()) {
        return false;
    }
    return true;
}

void ItemBaseGame3::moveDown() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto moveAction = MoveBy::create(4.0f, Vec2(0, -visibleSize.height));
    auto sequence = Sequence::create(moveAction, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr);
    this->runAction(sequence);
}
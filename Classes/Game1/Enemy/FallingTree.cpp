#include"Game1/Enemy/FallingTree.h"
#include"cocos2d.h"
using namespace cocos2d;

bool FallingTree::init() {
    if (!Sprite::init()) {
        return false;
    }
    return true;
}

void FallingTree::spawn(const cocos2d::Vec2& startPosition) {
    this->setPosition(startPosition);
    // Tree falling down from top
    auto moveDown = MoveBy::create(3.0f, Vec2(0, -Director::getInstance()->getVisibleSize().height));
    this->runAction(moveDown);
}

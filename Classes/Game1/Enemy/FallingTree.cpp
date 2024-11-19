#include "FallingTree.h"
#include "FallingTreePool.h"
#include "cocos2d.h"

USING_NS_CC;

FallingTree* FallingTree::create() {
    FallingTree* ret = new (std::nothrow) FallingTree();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FallingTree::init() {
    if (!FallingObject::init()) {
        return false;
    }
    return true;
}

void FallingTree::initAnimation() {
    _currentSprite = Sprite::create("assets_game/enemies/falling_tree.png");
    this->addChild(_currentSprite);

    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.12f);
    _currentSprite->setScale(_spriteScale);
}

FallingTree::~FallingTree() {
    // Clean up if necessary
}

void FallingTree::returnToPool() {
    FallingObject::reset();
    FallingTreePool::getInstance()->returnEnemy(this);
}
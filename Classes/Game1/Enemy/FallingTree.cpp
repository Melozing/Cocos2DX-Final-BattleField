#include "Game1/Enemy/FallingTree.h"
#include "Game1/Enemy/FallingTreePool.h"

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
    if (!Sprite::init()) {
        return false;
    }

    // Load the sprite
    _currentSprite = Sprite::create("assets_game/enemies/falling_tree.png");
    this->addChild(_currentSprite);

    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.12f); // Scale for tree
    _currentSprite->setScale(_spriteScale);

    // Schedule update to run every frame
    this->scheduleUpdate();

    return true;
}

void FallingTree::spawn(const Vec2& startPosition) {
    this->setPosition(startPosition);

    // Define target position off-screen at the bottom
    Vec2 endPosition = Vec2(startPosition.x, -SpriteController::calculateScreenRatio(Constants::FALLINGROCK_ITEMS_OFFSET));

    // Calculate distance and duration based on speed
    float distance = startPosition.distance(endPosition);
    float duration = distance / _speed;

    // Move down action
    auto moveDown = MoveTo::create(duration, endPosition);

    // Callback to remove tree when it moves off-screen
    auto removeTree = CallFunc::create([this]() {
        this->removeWhenOutOfScreen();
        });

    // Run move action and remove when done
    this->runAction(Sequence::create(moveDown, removeTree, nullptr));
}

void FallingTree::update(float delta) {
    this->removeWhenOutOfScreen();
}

void FallingTree::removeWhenOutOfScreen() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (this->getPosition().y < -this->getContentSize().height - SpriteController::calculateScreenRatio(Constants::FALLINGROCK_ITEMS_OFFSET)) {
        this->stopAllActions();
        this->removeFromParentAndCleanup(false);
        FallingTreePool::getInstance()->returnEnemy(this);
    }
}

Size FallingTree::GetSize() {
    return GetContentSizeSprite(_currentSprite);
}

void FallingTree::reset() {
    // Reset any necessary properties here
}

FallingTree::~FallingTree() {
    // Clean up if necessary
}

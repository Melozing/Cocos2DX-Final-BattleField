#include "Game1/Items/CollectibleItem.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"
USING_NS_CC;

bool CollectibleItem::init() {
    if (!Node::init()) {
        return false;
    }
    return true;
}

void CollectibleItem::spawn(const Vec2& startPosition) {
    this->setPosition(startPosition);

    // Define target position off-screen at the bottom
    Vec2 endPosition = Vec2(startPosition.x, -SpriteController::calculateScreenRatio(Constants::FALLINGROCK_ITEMS_OFFSET));

    // Calculate distance and duration based on speed
    float distance = startPosition.distance(endPosition);
    float duration = distance / _speed;

    // Move down action
    auto moveDown = MoveTo::create(duration, endPosition);

    // Callback to remove item when it moves off-screen
    auto removeItem = CallFunc::create([this]() {
        this->removeWhenOutOfScreen();
        });

    // Run move action and remove when done
    this->runAction(Sequence::create(moveDown, removeItem, nullptr));
}

Size CollectibleItem::GetSize() const {
    return _currentSprite->getContentSize();
}

// Remove the update method
 void CollectibleItem::update(float delta) {
     removeWhenOutOfScreen();
 }

void CollectibleItem::removeWhenOutOfScreen() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (this->getPosition().y < -this->getContentSize().height - 50.0f) {
        this->removeFromParentAndCleanup(true);
    }
}

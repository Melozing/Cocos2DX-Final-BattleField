#include "Game1/Items/CollectibleItem.h"
#include "Controller/SpriteController.h"
USING_NS_CC;

bool CollectibleItem::init() {
    if (!Node::init()) {
        return false;
    }
    return true;
}

void CollectibleItem::spawn(const Vec2& startPosition) {
    this->setPosition(startPosition);
}

Size CollectibleItem::GetSize() const {
    return _currentSprite->getContentSize();
}

void CollectibleItem::update(float delta) {
    Vec2 currentPosition = this->getPosition();
    Vec2 movement = Vec2(0, -_speed * delta);
    this->setPosition(currentPosition + movement);
    removeWhenOutOfScreen();
}

void CollectibleItem::removeWhenOutOfScreen() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (this->getPosition().y < -this->getContentSize().height - 50.0f) {
        this->removeFromParentAndCleanup(true);
    }
}

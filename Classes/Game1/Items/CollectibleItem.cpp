// CollectibleItem.cpp
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

    // Set initial position and run move action and remove when done
    this->setPosition(startPosition);
    this->runAction(Sequence::create(moveDown, removeItem, nullptr));
}

Size CollectibleItem::GetSize() const {
    return _currentSprite->getContentSize();
}

void CollectibleItem::update(float delta) {
    removeWhenOutOfScreen();
}

void CollectibleItem::removeWhenOutOfScreen() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (this->getPosition().y < -this->getContentSize().height - 50.0f) {
        this->returnToPool(); // Return to pool instead of removing
    }
}

void CollectibleItem::playEffectAndRemove() {
    //this->setVisible(true);
    //this->setOpacity(255);

    //// Scale up to _scaleFactor times over 0.5 seconds
    //auto scaleUp = ScaleTo::create(0.5f, _scaleFactor);

    //// Fade out over 0.5 seconds
    //auto fadeOut = FadeOut::create(0.5f);

    //// Run the scale and fade actions simultaneously, then return to pool
    //_currentSprite->runAction(Sequence::create(Spawn::create(scaleUp, fadeOut, nullptr), nullptr));
}

void CollectibleItem::returnToPool() {
    // Override this function in derived classes
}

Size CollectibleItem::getScaledSize() const {
    Size originalSize = _currentSprite->getContentSize();
    return Size(originalSize.width * _spriteScale, originalSize.height * _spriteScale);
}

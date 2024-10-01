#include "Game1/Items/CollectibleItem.h"
USING_NS_CC;

CollectibleItem* CollectibleItem::create() {
    CollectibleItem* ret = new (std::nothrow) CollectibleItem();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CollectibleItem::init() {
    // Load sprite frames for the item
    _currentSprite = Sprite::create("assets_game/items/items_health.png");
    this->addChild(_currentSprite);
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.08f); // Scale for rock
    _currentSprite->setScale(_spriteScale);
    // Schedule update to run every frame
    this->scheduleUpdate();

    return true;
}

void CollectibleItem::spawn(const Vec2& startPosition) {
    this->setPosition(startPosition);
}

Size CollectibleItem::GetSize() {
    return GetContentSizeSprite(_currentSprite);
}

void CollectibleItem::update(float delta) {
    Vec2 currentPosition = this->getPosition();
    Vec2 movement = Vec2(0, -_speed * delta); // Moving downwards
    this->setPosition(currentPosition + movement);

    // Remove item if it goes off-screen
    removeWhenOutOfScreen();
}


CollectibleItem::~CollectibleItem() {
    // Clean up resources if necessary
}

void CollectibleItem::removeWhenOutOfScreen() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (this->getPosition().y < -this->getContentSize().height - 50.0f) {
        this->removeFromParentAndCleanup(true);
    }
}

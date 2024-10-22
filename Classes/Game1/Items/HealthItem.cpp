#include "HealthItem.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/SpriteController.h"
#include "HealthItemPool.h"

USING_NS_CC;

HealthItem* HealthItem::create() {
    HealthItem* ret = new (std::nothrow) HealthItem();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool HealthItem::init() {
    if (!Node::init()) return false;
    _currentSprite = Sprite::create("assets_game/items/items_health.png");
    _currentSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.08f);
    _currentSprite->setScale(_spriteScale);
    this->addChild(_currentSprite);
    this->scheduleUpdate();
    return true;
}

void HealthItem::applyEffect() {
    PlayerAttributes::getInstance().SetHealth(PlayerAttributes::getInstance().GetHealth() + 1);
    this->playEffectAndRemove();
}

Size HealthItem::getScaledSize() const {
    Size originalSize = _currentSprite->getContentSize();
    return Size(originalSize.width * _spriteScale, originalSize.height * _spriteScale);
}

void HealthItem::returnToPool() {
    this->removeFromParentAndCleanup(false);
    HealthItemPool::getInstance()->returnItem(this);
}

void HealthItem::reset() {
    // Reset the state of the HealthItem
    this->setVisible(true);
    this->setPosition(Vec2::ZERO);
}

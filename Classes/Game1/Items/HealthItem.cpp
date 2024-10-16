#include "HealthItem.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/SpriteController.h"
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
    this->addChild(_currentSprite);
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.08f);
    _currentSprite->setScale(_spriteScale);
    this->scheduleUpdate();
    return true;
}

void HealthItem::applyEffect() {
    PlayerAttributes::getInstance().SetHealth(PlayerAttributes::getInstance().GetHealth() + 1);
}

Size HealthItem::getScaledSize() const {
    Size originalSize = _currentSprite->getContentSize();
    return Size(originalSize.width * _spriteScale, originalSize.height * _spriteScale);
}

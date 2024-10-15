#include "Game1/Items/AmmoItem.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

AmmoItem* AmmoItem::create() {
    AmmoItem* ret = new (std::nothrow) AmmoItem();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool AmmoItem::init() {
    if (!Node::init()) return false;
    _currentSprite = Sprite::create("assets_game/items/items_superpower.png");
    this->addChild(_currentSprite);
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.08f);
    _currentSprite->setScale(_spriteScale);
    this->scheduleUpdate();
    return true;
}

void AmmoItem::applyEffect() {
    PlayerAttributes::getInstance().SetAmmo(PlayerAttributes::getInstance().GetAmmo() + 1);
}

Size AmmoItem::getScaledSize() const {
    Size originalSize = _currentSprite->getContentSize();
    return Size(originalSize.width * _spriteScale, originalSize.height * _spriteScale);
}

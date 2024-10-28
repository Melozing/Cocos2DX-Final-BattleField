// AmmoItem.cpp
#include "AmmoItem.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/SpriteController.h"
#include "AmmoItemPool.h"

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
    _currentSprite = Sprite::create("assets_game/items/ao.png");
    _currentSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.07f);
    //_currentSprite->setScale(_spriteScale);
    this->addChild(_currentSprite);
    this->setScale(_spriteScale);
    this->scheduleUpdate();
    this->initPhysicsBody();
    return true;
}

void AmmoItem::initPhysicsBody() {
    // Create and attach a physics body
    auto physicsBody = PhysicsBody::createBox(_currentSprite->getContentSize());
    physicsBody->setCollisionBitmask(0x03);
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    this->setPhysicsBody(physicsBody);

}

void AmmoItem::applyEffect() {
    PlayerAttributes::getInstance().SetAmmo(PlayerAttributes::getInstance().GetAmmo() + 1);
    this->setVisible(true);
    this->setOpacity(255);

    // Scale up to _scaleFactor times over 0.5 seconds
    auto scaleUp = ScaleTo::create(0.5f, _scaleFactor);

    // Fade out over 0.5 seconds
    auto fadeOut = FadeOut::create(0.5f);

    // Run the scale and fade actions simultaneously
    auto scaleAndFade = Spawn::create(scaleUp, fadeOut, nullptr);

    // Call playEffectAndRemove after scale and fade actions are complete
    auto callPlayEffectAndRemove = CallFunc::create([this]() {
        this->playEffectAndRemove();
        });

    // Create a sequence to run scaleAndFade, then callPlayEffectAndRemove
    auto sequence = Sequence::create(scaleAndFade, callPlayEffectAndRemove, nullptr);

    // Run the sequence on the sprite
    _currentSprite->runAction(sequence);
}


void AmmoItem::playEffectAndRemove() {
    if (this->getPhysicsBody()) {
        this->removeComponent(this->getPhysicsBody()); // Remove PhysicsBody
    }
	this->returnToPool();
}

Size AmmoItem::getScaledSize() const {
    return SpriteController::GetContentSizeSprite(_currentSprite);;
}

void AmmoItem::returnToPool() {
    this->stopAllActions();
    this->removeFromParentAndCleanup(false);
    AmmoItemPool::getInstance()->returnItem(this);
}

void AmmoItem::reset() {
    // Reset the state of the AmmoItem
    this->setOpacity(255);
    this->setVisible(true);
    this->initPhysicsBody();
}

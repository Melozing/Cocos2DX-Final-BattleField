// HealthItem.cpp
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
    _currentSprite = Sprite::create("assets_game/items/cao_sao_vang.png");
    _currentSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.07f);
    this->addChild(_currentSprite);
    this->setScale(_spriteScale); 
    this->scheduleUpdate();
    this->initPhysicsBody();
    return true;
}


void HealthItem::initPhysicsBody() {
    // Create and attach a physics body
    auto physicsBody = PhysicsBody::createBox(_currentSprite->getContentSize());
    physicsBody->setCollisionBitmask(0x03);
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    this->setPhysicsBody(physicsBody);
}

void HealthItem::applyEffect() {
    PlayerAttributes::getInstance().SetHealth(PlayerAttributes::getInstance().GetHealth() + 1);
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


Size HealthItem::getScaledSize() const {
    return SpriteController::GetContentSizeSprite(_currentSprite);;
}

void HealthItem::playEffectAndRemove() {
    if (this->getPhysicsBody()) {
        this->removeComponent(this->getPhysicsBody()); // Remove PhysicsBody
    }
    this->returnToPool();
}

void HealthItem::returnToPool() {
    this->stopAllActions();
    this->removeFromParentAndCleanup(false);
    HealthItemPool::getInstance()->returnItem(this);
}

void HealthItem::reset() {
    // Reset the state of the HealthItem
    auto fadeIn = FadeIn::create(0.5f);
    this->setOpacity(255);
    this->setVisible(true);
    this->initPhysicsBody();
    _currentSprite->runAction(fadeIn);
}

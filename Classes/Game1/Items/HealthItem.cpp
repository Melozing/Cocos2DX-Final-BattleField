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
    this->scheduleUpdate();
    this->initAnimation(); 
    this->initPhysicsBody();
    return true;
}

void HealthItem::initAnimation() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/items/Health.plist");

    auto spriteBatchNode = SpriteBatchNode::create("assets_game/items/Health.png");
    this->addChild(spriteBatchNode);

    _currentSprite = Sprite::createWithSpriteFrameName("Health1.png");
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, Constants::ITEM_SIZE_RATIO);
    _currentSprite->setScale(_spriteScale);

    spriteBatchNode->addChild(_currentSprite);

    auto animateCharac = Animate::create(SpriteController::createForwardReverseAnimation("Health", 30, 0.03f));
    _currentSprite->runAction(RepeatForever::create(animateCharac));
}

void HealthItem::initPhysicsBody() {
    // Create and attach a physics body
    Size reducedSize = Size(_currentSprite->getContentSize().width * 0.65, _currentSprite->getContentSize().height * 0.65);
    auto physicsBody = PhysicsBody::createBox(reducedSize);
    physicsBody->setCollisionBitmask(0x03);
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    this->setPhysicsBody(physicsBody);
}

void HealthItem::applyEffect() {
    PlayerAttributes::getInstance().SetHealth(PlayerAttributes::getInstance().GetHealth() + 1);
    this->setVisible(true);
    this->setOpacity(255);

    _scaleFactor = SpriteController::updateSpriteScale(_currentSprite, Constants::ITEM_SCALE_FACTOR + 0.03f);
    // Scale up to _scaleFactor times over 0.5 seconds
    auto scaleUp = ScaleTo::create(Constants::ITEM_EFFECT_DURATION, _scaleFactor);

    // Fade out over 0.5 seconds
    auto fadeOut = FadeOut::create(Constants::ITEM_EFFECT_DURATION);

    // Run the scale and fade actions simultaneously
    auto scaleAndFade = Spawn::create(scaleUp, fadeOut, nullptr);

    // Call playEffectAndRemove after scale and fade actions are complete
    auto callPlayEffectAndRemove = CallFunc::create([this]() {
        this->playEffectAndRemove();
        });
    auto callRemovePhysicBody = CallFunc::create([this]() {
        this->RemovePhysicBody();
        });

    // Create a sequence to run scaleAndFade, then callPlayEffectAndRemove
    auto sequence = Sequence::create(callRemovePhysicBody, scaleAndFade, callPlayEffectAndRemove, nullptr);

    // Run the sequence on the sprite
    _currentSprite->runAction(sequence);
}

Size HealthItem::getScaledSize() const {
    return SpriteController::GetContentSizeSprite(_currentSprite);;
}

void HealthItem::RemovePhysicBody() {
    if (this->getPhysicsBody()) {
        this->removeComponent(this->getPhysicsBody()); // Remove PhysicsBody
    }
}

void HealthItem::playEffectAndRemove() {
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
    this->initAnimation();
    this->initPhysicsBody();
    _currentSprite->runAction(fadeIn);
}

// ShieldSkillItem.cpp
#include "ShieldSkillItem.h"
#include "Game1/Skills/ShieldSkillItemPool.h"
#include "Game1/Player/PlayerGame1.h"

USING_NS_CC;

ShieldSkillItem* ShieldSkillItem::create() {
    ShieldSkillItem* shield = new (std::nothrow) ShieldSkillItem();
    if (shield && shield->init()) {
        shield->autorelease();
        return shield;
    }
    CC_SAFE_DELETE(shield);
    return nullptr;
}

bool ShieldSkillItem::init() {
    if (!Sprite::init()) {
        return false;
    }

    this->initAnimation();
    return true;
}

void ShieldSkillItem::onExit() {
    Sprite::onExit();
}

void ShieldSkillItem::initAnimation() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/shield.plist");

    auto spriteBatchNode = SpriteBatchNode::create("assets_game/player/shield.png");
    this->addChild(spriteBatchNode);

    _shieldSprite = Sprite::createWithSpriteFrameName("shield1.png");
    _shieldSprite->setScale(SpriteController::updateSpriteScale(_shieldSprite, 0.13f));
    this->addChild(_shieldSprite);

    auto animateShield = Animate::create(createAnimation("shield", 15, 0.07f));
    _shieldSprite->runAction(RepeatForever::create(animateShield));
}

void ShieldSkillItem::reset() {
    this->setVisible(false);
    this->setOpacity(255);
    this->stopAllActions(); // Stop any ongoing actions
    this->playAnimation();
}

void ShieldSkillItem::playAnimation() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/shield.plist");
    auto animation = createAnimation("shield", 15, 0.07f);
    auto animate = Animate::create(animation);
    _shieldSprite->runAction(RepeatForever::create(animate));
}

void ShieldSkillItem::activate(float duration) {
    this->setVisible(true);
    this->setOpacity(0); // Start with 0 opacity for fade-in effect
    auto fadeIn = FadeIn::create(0.5f);
    this->runAction(fadeIn);
    this->runAction(Sequence::create(
        DelayTime::create(duration),
        CallFunc::create([this]() { this->deactivate(); }),
        nullptr
    ));
}

void ShieldSkillItem::deactivate() {
    auto fadeOut = FadeOut::create(0.5f);
    this->runAction(Sequence::create(
        fadeOut,
        CallFunc::create([this]() {
            this->setVisible(false);
            ShieldSkillItemPool::getInstance()->returnItem(this);
            if (auto player = dynamic_cast<PlayerGame1*>(this->getParent())) {
                player->_hasShield = false; // Reset the player's shield status
            }
            }),
        nullptr
    ));
}


// ShieldSkillItem.cpp
#include "ShieldSkillItem.h"
#include "Game1/Skills/ShieldSkillItemPool.h"

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

void ShieldSkillItem::initAnimation() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/shield.plist");

    auto spriteBatchNode = SpriteBatchNode::create("assets_game/player/shield.png");
    this->addChild(spriteBatchNode);

    _shieldSprite = Sprite::createWithSpriteFrameName("shield1.png");
    _shieldSprite->setScale(SpriteController::updateSpriteScale(_shieldSprite, 0.13f));

    //spriteBatchNode->addChild(_shieldSprite);

    auto animateShield = Animate::create(createAnimation("shield", 15, 0.07f));
    _shieldSprite->runAction(RepeatForever::create(animateShield));
}

void ShieldSkillItem::reset() {
    this->playAnimation();
}

void ShieldSkillItem::playAnimation() {
    this->setVisible(true);
    this->setOpacity(255);
    auto fadein = FadeIn::create(0.5f);
    this->runAction(fadein);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/shield.plist");
    auto animation = createAnimation("shield", 15, 0.07f);
    auto animate = Animate::create(animation);
    this->runAction(RepeatForever::create(animate));
}

void ShieldSkillItem::activate(float duration) {
    this->setVisible(true);
    this->setOpacity(255);
    this->runAction(Sequence::create(
        DelayTime::create(duration),
        CallFunc::create([this]() { this->deactivate(); }),
        nullptr
    ));
}

void ShieldSkillItem::deactivate() {
    this->setVisible(true);
    this->setOpacity(255);
    auto fadeOut = FadeOut::create(0.5f);
    this->runAction(fadeOut);
    ShieldSkillItemPool::getInstance()->returnItem(this);
}

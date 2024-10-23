// PlayerGame1.cpp
#include "Game1/Player/PlayerGame1.h"
#include "Constants/Constants.h"
#include "cocos2d.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

bool PlayerGame1::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    _health = 3;

    float restrictedWidth = SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float restrictedHeight = visibleSize.height - SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDHEIGHT);

    float centerX = visibleSize.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    minX = centerX - halfRestrictedWidth;
    maxX = centerX + halfRestrictedWidth;

    float centerY = visibleSize.height / 2;
    float halfRestrictedHeight = restrictedHeight / 2;
    minY = centerY - halfRestrictedHeight;
    maxY = centerY + halfRestrictedHeight;

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame1::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame1::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    playerMovement = new PlayerMovement(this, Constants::PLAYER_MOVESPEED, minX, maxX, minY, maxY);

    return true;
}

void PlayerGame1::takeDamage()
{
    if (canTakeDamage()) {
        _health -= 1;
        _lastDamageTime = Director::getInstance()->getTotalFrames();

        playDamageEffect();

        if (_health <= 0) {
            CCLOG("Game Over");
        }
        else {
            CCLOG("Player health: %d", _health);
        }
    }
}

bool PlayerGame1::canTakeDamage()
{
    float currentTime = Director::getInstance()->getTotalFrames();
    return (currentTime - _lastDamageTime) >= _damageCooldown * 60 && !attributes->IsDead();
}

PlayerGame1* PlayerGame1::createPlayer()
{
    PlayerGame1* player = new (std::nothrow) PlayerGame1();
    if (player && player->init())
    {
        player->autorelease();
        player->initAnimation();
        player->initShield();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

void PlayerGame1::initAnimation()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/Canon.plist");

    auto spriteBatchNode = SpriteBatchNode::create("assets_game/player/Canon.png");
    this->addChild(spriteBatchNode);

    modelCharac = Sprite::createWithSpriteFrameName("Canon1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.13f));

    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation("Canon", 15, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void PlayerGame1::initShield()
{
    _shieldItem = ShieldSkillItemPool::getInstance()->getItem();
    if (_shieldItem) {
        _shieldItem->setVisible(false);
        this->addChild(_shieldItem);
    }
}

void PlayerGame1::activateShield(float duration) {
    if (_shieldItem && !_hasShield) {
        _hasShield = true;
        _shieldItem->activate(duration);
    }
}


void PlayerGame1::deactivateShield() {
    if (_shieldItem && _hasShield) {
        _hasShield = false;
        _shieldItem->deactivate(); 
        _shieldItem = nullptr; // Reset the shield item reference
    }
}


bool PlayerGame1::hasShield() const
{
    return _hasShield;
}

Size PlayerGame1::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void PlayerGame1::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    playerMovement->onKeyPressed(keyCode);
}

void PlayerGame1::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    playerMovement->onKeyReleased(keyCode);
}

void PlayerGame1::update(float delta)
{
    playerMovement->update(delta);
}

void PlayerGame1::playDamageEffect() {
    auto blinkAction = Blink::create(1.0f, 3);
    auto tintToRed = TintTo::create(0.1f, 255, 0, 0);
    auto tintToNormal = TintTo::create(0.1f, 255, 255, 255);
    auto sequence = Sequence::create(tintToRed, blinkAction, tintToNormal, nullptr);
    modelCharac->runAction(sequence);
}

void PlayerGame1::playHealthIncreaseEffect() {
    auto tintToGreen = TintTo::create(0.1f, 0, 255, 0);
    auto tintToNormal = TintTo::create(0.1f, 255, 255, 255);
    auto sequence = Sequence::create(tintToGreen, tintToNormal, nullptr);
    modelCharac->runAction(sequence);
}
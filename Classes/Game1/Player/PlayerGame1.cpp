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

    // Define the width and height of the restricted area in the center of the screen
    float restrictedWidth = SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH); // The total width of the area where movement is allowed

    // Get visible size of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float restrictedHeight = visibleSize.height - SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDHEIGHT); // The total height of the area where movement is allowed

    // Calculate boundaries for horizontal movement
    float centerX = visibleSize.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    minX = centerX - halfRestrictedWidth;
    maxX = centerX + halfRestrictedWidth;

    // Calculate boundaries for vertical movement
    float centerY = visibleSize.height / 2;
    float halfRestrictedHeight = restrictedHeight / 2;
    minY = centerY - halfRestrictedHeight;
    maxY = centerY + halfRestrictedHeight;

    // Add keyboard event listener
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

        // Play damage effect
        playDamageEffect();

        if (_health <= 0) {
            CCLOG("Game Over");
        }
        else {
            CCLOG("Player health: %d", _health);
        }
    }
}

// Update the canTakeDamage function to use PlayerAttributes' health state
bool PlayerGame1::canTakeDamage()
{
    float currentTime = Director::getInstance()->getTotalFrames();
    // Allow damage if enough time has passed and the player is not dead
    return (currentTime - _lastDamageTime) >= _damageCooldown * 60 && !attributes->IsDead();
}


PlayerGame1* PlayerGame1::createPlayer()
{
    PlayerGame1* player = new (std::nothrow) PlayerGame1();
    if (player && player->init())
    {
        player->autorelease();
        player->initAnimation();
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
    // Create a blink action
    auto blinkAction = Blink::create(1.0f, 3); // Blink for 1 second, 3 times

    // Create a color change action to red
    auto tintToRed = TintTo::create(0.1f, 255, 0, 0); // Change to red over 0.1 seconds

    // Create a color change action back to the original color
    auto tintToNormal = TintTo::create(0.1f, 255, 255, 255); // Change back to normal over 0.1 seconds

    // Create a sequence of actions: tint to red, blink, tint back to normal
    auto damageEffect = Sequence::create(tintToRed, blinkAction, tintToNormal, nullptr);

    // Run the sequence on the player sprite
    modelCharac->runAction(damageEffect);
}

void PlayerGame1::playHealthIncreaseEffect() {
    // Create a color change action to green
    auto tintToGreen = TintTo::create(0.1f, 0, 255, 0); // Change to green over 0.1 seconds

    // Create a color change action back to the original color
    auto tintToNormal = TintTo::create(0.1f, 255, 255, 255); // Change back to normal over 0.1 seconds

    // Create a sequence of actions: tint to green, tint back to normal
    auto healthEffect = Sequence::create(tintToGreen, tintToNormal, nullptr);

    // Run the sequence on the player sprite
    modelCharac->runAction(healthEffect);
}
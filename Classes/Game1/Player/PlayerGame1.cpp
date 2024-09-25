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

    return true;
}

void PlayerGame1::takeDamage()
{
    if (canTakeDamage()) {
        _health -= 1; 
        _lastDamageTime = Director::getInstance()->getTotalFrames(); 

        if (_health <= 0) {
            CCLOG("Game Over");
        }
        else {
            CCLOG("Player health: %d", _health);
        }
    }
}

// Add a new method to handle collision with enemies
void PlayerGame1::OnCollisionWithEnemy(int enemyDamage)
{
    // Check if player can take damage
    if (canTakeDamage() && !attributes->IsDead())
    {
        // Reduce health based on the enemy's damage
        attributes->TakeDamage(enemyDamage);
        _lastDamageTime = Director::getInstance()->getTotalFrames(); // Update the last damage time

        // Log the player's current health
        CCLOG("Player health: %d", attributes->GetHealth());

        // Check if player is dead
        if (attributes->IsDead())
        {
            // Call GameOver method from GameController singleton
            GameController::getInstance()->GameOver(attributes);
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
    SpriteController::updateSpriteScale(modelCharac, 0.05f);
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.15f));

    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation("Canon", 15, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

Size PlayerGame1::GetSize() {
    return GetContentSizeSprite(modelCharac);
}


void PlayerGame1::moveUp()
{
    Vec2 position = this->getPosition();
    if (position.y + Constants::PLAYER_MOVESPEED < maxY) {
        position.y += Constants::PLAYER_MOVESPEED;
        this->setPosition(position);
    }
}

void PlayerGame1::moveDown()
{
    Vec2 position = this->getPosition();
    if (position.y - Constants::PLAYER_MOVESPEED > minY) {
        position.y -= Constants::PLAYER_MOVESPEED * 1.5f;
        this->setPosition(position);
    }
}

void PlayerGame1::moveLeft()
{
    Vec2 position = this->getPosition();
    if (position.x - Constants::PLAYER_MOVESPEED > minX) {
        position.x -= Constants::PLAYER_MOVESPEED;
        this->setPosition(position);
    }
}

void PlayerGame1::moveRight()
{
    Vec2 position = this->getPosition();
    if (position.x + Constants::PLAYER_MOVESPEED < maxX) {
        position.x += Constants::PLAYER_MOVESPEED;
        this->setPosition(position);
    }
}

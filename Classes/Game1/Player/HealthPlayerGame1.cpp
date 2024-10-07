#include "Game1/Player/HealthPlayerGame1.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"

HealthPlayerGame1* HealthPlayerGame1::createHealth()
{
    HealthPlayerGame1* health = new (std::nothrow) HealthPlayerGame1();
    if (health && health->init())
    {
        health->autorelease();
        return health;
    }
    CC_SAFE_DELETE(health);
    return nullptr;
}

void HealthPlayerGame1::initHealthSprites(int health) {
    auto visibleSize = Director::getInstance()->getVisibleSize(); // Get the visible size of the window
    for (int i = 0; i < health; i++) { // Loop for player health sprites
        auto healthSprite = Sprite::create("assets_game/player/HP_Dot.png"); // Load health sprite
        healthSprite->setScale(SpriteController::updateSpriteScale(healthSprite, 0.1f));
        healthSprite->setPosition(Vec2(SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_X_START) + i * SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_X), visibleSize.height - SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_Y))); // Set position for each health sprite
        this->addChild(healthSprite); // Add sprite to the scene
        _healthSprites.push_back(healthSprite); // Store sprite in vector
    }
}

void HealthPlayerGame1::updateHealthSprites(int health) {
    int currentHealth = health;

    // Ensure the number of health sprites matches the current health
    if (currentHealth > _healthSprites.size()) {
        int additionalSprites = currentHealth - _healthSprites.size();
        auto visibleSize = Director::getInstance()->getVisibleSize();
        for (int i = 0; i < additionalSprites; ++i) {
            auto healthSprite = Sprite::create("assets_game/player/HP_Dot.png");
            healthSprite->setScale(SpriteController::updateSpriteScale(healthSprite, 0.1f));
            healthSprite->setPosition(Vec2(SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_X_START) + (_healthSprites.size() + i) * SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_X), visibleSize.height - SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_Y)));
            this->addChild(healthSprite);
            _healthSprites.push_back(healthSprite);
        }
    }

    // Update visibility of health sprites
    for (int i = 0; i < _healthSprites.size(); i++) {
        if (i < currentHealth) {
            _healthSprites[i]->setVisible(true); // Show sprite if player has health
            _healthSprites[i]->setOpacity(255); // Ensure the sprite is fully visible
        }
        else {
            auto fadeOut = FadeOut::create(0.5f); // 0.5 second to fade out
            auto hideSprite = CallFunc::create([this, i]() {
                _healthSprites[i]->setVisible(false); // Hide the sprite after fade out
                });
            auto sequence = Sequence::create(fadeOut, hideSprite, nullptr); // Create sequence
            _healthSprites[i]->runAction(sequence); // Run fade out action
        }
    }
}

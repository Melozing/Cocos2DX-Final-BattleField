#include "UpgradeBulletItemGame3.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

bool UpgradeBulletItemGame3::init() {
    if (!ItemBaseGame3::init()) {
        return false;
    }
    this->setTexture("assets_game/items/level_up.png");
    this->setScale(SpriteController::updateSpriteScale(this, 0.07f)); 
    return true;
}

void UpgradeBulletItemGame3::applyEffect() {
    // Implement the effect of upgrading the bullet
}

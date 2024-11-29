#include "IncreaseBulletCountItemGame3.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

bool IncreaseBulletCountItemGame3::init() {
    if (!ItemBaseGame3::init()) {
        return false;
    }
    this->setTexture("assets_game/items/hop_dan.png");
    this->setScale(SpriteController::updateSpriteScale(this, 0.07f)); 
    return true;
}

void IncreaseBulletCountItemGame3::applyEffect() {
    // Implement the effect of increasing bullet count
}

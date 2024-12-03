#include "UpgradeBulletItemGame3.h"
#include "Controller/SpriteController.h"
#include "utils/PhysicsShapeCache.h"

USING_NS_CC;

bool UpgradeBulletItemGame3::init() {
    if (!ItemBaseGame3::init()) {
        return false;
    }
    this->setTexture("assets_game/items/level_up.png");
    this->setScale(SpriteController::updateSpriteScale(this, 0.07f));

    // Set the physics body info
    this->setPhysicsBodyInfo("physicsBody/UpgradeBulletItem.plist", "UpgradeBulletItem");
    initialScale = this->getScale();

    return true;
}

void UpgradeBulletItemGame3::applyEffect() {
    // Implement the effect of upgrading the bullet
}
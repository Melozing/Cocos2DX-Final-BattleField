#include "EnemyBase.h"
#include "Game2/Player/PlayerGame2.h"
#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "FX/Explodable.h"
#include "Game3/Items/ItemBaseGame3.h"
#include "Game3/Items/ItemPoolGane3.h"

USING_NS_CC;

bool EnemyBase::init() {
    if (!Sprite::init()) {
        return false;
    }
    this->scheduleUpdate();
    return true;
}

void EnemyBase::update(float delta) {
    moveToPlayer();
    updateRotationToPlayer();
}

void EnemyBase::reset() {
    this->setRotation(0);
    this->stopAllActions();
    this->setVisible(false);
    this->setPosition(Vec2::ZERO);
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
}

void EnemyBase::updateRotationToPlayer() {
    // Uncomment and implement if needed
    // auto playerPos = getPlayerPosition();
    // auto direction = playerPos - this->getPosition();
    // float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));
    // this->setRotation(-angle);
}

void EnemyBase::moveToPlayer() {
    // Uncomment and implement if needed
    // auto playerPos = getPlayerPosition();
    // auto direction = playerPos - this->getPosition();
    // direction.normalize();
    // this->setPosition(this->getPosition() + direction * Constants::EnemySpeed);
}

void EnemyBase::explode() {
    // Drop a random item
    this->stopAllActions();

    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    this->setVisible(false);
    SoundController::getInstance()->playSoundEffect(Constants::EnemyCrepExplodeSFX);
    auto explosion = Explosion::create(this->getPosition(), [this]() {
        this->returnToPool();
        });
    if (this->getParent() != nullptr) {
        this->getParent()->addChild(explosion);
    }
}

void EnemyBase::dropRandomItem() {
    // Define the drop chance (e.g., 15% chance to drop an item)
    float dropChance = 0.15f;

    // Generate a random number between 0 and 1
    float randomValue = CCRANDOM_0_1();

    // Check if the random value is less than the drop chance
    if (randomValue < dropChance) {
        // Proceed to drop an item
        int randomItem = random(0, 1); // Assuming 2 types of items
        ItemBaseGame3* item = nullptr;

        switch (randomItem) {
        case 0:
            item = HealthRecoveryItemPool::getInstance()->getItem();
            break;
        case 1:
            item = IncreaseBulletCountItemPool::getInstance()->getItem();
            break;
        }

        if (item) {
            if (this->getParent() != nullptr) {
                this->getParent()->addChild(item);
            }
            item->setStartPosition(this->getPosition());
            item->moveDown();
        }
    }
}

void EnemyBase::returnToPool() {
    this->stopAllActions();
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);
    this->reset();
    // Add logic to return to the appropriate pool if needed
}

void EnemyBase::resetSprite() {
    if (this->modelCharac) {
        this->modelCharac->setFlippedX(false);
    }
    this->stopAllActions();
    this->setVisible(true);
}

// Uncomment and implement if needed
// Vec2 EnemyBase::getPlayerPosition() {
//     // Assuming PlayerGame2 is a singleton or can be accessed globally
//     return PlayerGame2::getInstance()->getPosition();
// }


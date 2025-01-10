#include "BunkerGame2.h"
#include "Controller/SpriteController.h"
#include "Manager/ObjectPoolGame2.h"
#include "utils/PhysicsShapeCache.h"
#include "Game2/Enemy/SniperEnemyGame2.h" // Include SniperEnemyGame2

USING_NS_CC;

BunkerGame2* BunkerGame2::create() {
    BunkerGame2* ret = new (std::nothrow) BunkerGame2();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool BunkerGame2::init() {
    if (!Sprite::init()) {
        return false;
    }

    modalSprite = Sprite::create("assets_game/enemies/bunker.png");
    this->setScale(SpriteController::updateSpriteScale(modalSprite, 0.1f));
    this->setAnchorPoint(Vec2(0.5f, 0));
    this->addChild(modalSprite);
    this->createPhysicsBody();
    this->schedule([this](float dt) { this->spawnEnemy(dt); }, 3.0f, "spawn_enemy_key");
    return true;
}

void BunkerGame2::setTargetCannon(Cannon* cannon) {
    this->targetCannon = cannon;
}

void BunkerGame2::setPlayer(PlayerGame2* player) {
    this->targetPlayer = player;
}

void BunkerGame2::spawnEnemy(float dt) {
    int randomEnemyType = rand() % 3; // Random number between 0 and 2

    switch (randomEnemyType) {
    case 0: {
        auto enemy = SuicideBomberEnemyPool::getInstance()->getObject();
        if (enemy) {
            enemy->setTarget(targetCannon);
            enemy->setPosition(this->getPosition());

            if (enemy->getParent() != nullptr) {
                enemy->removeFromParent();
            }

            this->getParent()->addChild(enemy);
        }
        break;
    }
    case 1: {
        auto sniperEnemy = SniperEnemyGame2Pool::getInstance()->getObject();
        if (sniperEnemy) {
            sniperEnemy->setPosition(this->getPosition());
            sniperEnemy->setTarget(targetPlayer);

            if (sniperEnemy->getParent() != nullptr) {
                sniperEnemy->removeFromParent();
            }

            this->getParent()->addChild(sniperEnemy);
        }
        break;
    }
    case 2: {
        // Add logic for another enemy type if needed
        break;
    }
    default:
        break;
    }
}

Size BunkerGame2::GetSize() {
    return SpriteController::GetContentSizeSprite(modalSprite);
}

void BunkerGame2::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = modalSprite->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/BunkerGame2.plist", "BunkerGame2", originalSize, scaledSize);

    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setCollisionBitmask(0x01);
        this->setPhysicsBody(physicsBody);
    }
}

void BunkerGame2::reset() {
    this->unschedule("spawn_enemy_key");
}

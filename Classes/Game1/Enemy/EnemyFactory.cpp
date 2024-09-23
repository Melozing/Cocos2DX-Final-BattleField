#include "Game1/Enemy/EnemyFactory.h"
#include "Game1/Enemy/FlyingBullet.h"
#include "Game1/Enemy/FallingRock.h"
#include "Game1/Enemy/RandomBoom.h"

Enemy* EnemyFactory::spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position) {
    Enemy* enemy = nullptr;

    if (enemyType == "FlyingBullet") {
        enemy = FlyingBullet::create();
    }
    else if (enemyType == "FallingRock") {
        enemy = FallingRock::create();
    }
    else if (enemyType == "RandomBoom") {
        enemy = RandomBoom::create();
    }

    if (enemy) {
        enemy->setPosition(position);
    }

    return enemy;
}

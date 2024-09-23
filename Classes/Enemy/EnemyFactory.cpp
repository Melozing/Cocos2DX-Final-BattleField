#include "EnemyFactory.h"
#include "Enemy/FlyingBullet.h"
#include "Enemy/FallingRock.h"
#include "Enemy/RandomBoom.h"

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

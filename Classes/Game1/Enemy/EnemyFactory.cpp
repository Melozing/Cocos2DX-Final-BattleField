#include "Game1/Enemy/EnemyFactory.h"
#include "Game1/Enemy/FlyingBullet.h"
#include "Game1/Enemy/FallingRock.h"
#include "Game1/Enemy/RandomBoom.h"
#include "Game1/Enemy/FlyingBulletPool.h"
#include "Game1/Enemy/FallingRockPool.h"
#include "Game1/Enemy/RandomBoomPool.h"

EnemyGame1* EnemyFactoryGame1::spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position) {
    EnemyGame1* enemy = nullptr;

    if (enemyType == "FlyingBullet") {
        enemy = FlyingBulletPool::getInstance()->getEnemy();
    }
    else if (enemyType == "FallingRock") {
        enemy = FallingRockPool::getInstance()->getEnemy();
    }
    else if (enemyType == "RandomBoom") {
        enemy = RandomBoomPool::getInstance()->getEnemy();
    }

    if (enemy) {
        enemy->setPosition(position);
    }

    return enemy;
}

void EnemyFactoryGame1::returnEnemy(EnemyGame1* enemy) {
    if (auto flyingBullet = dynamic_cast<FlyingBullet*>(enemy)) {
        FlyingBulletPool::getInstance()->returnEnemy(flyingBullet);
    }
    else if (auto fallingRock = dynamic_cast<FallingRock*>(enemy)) {
        FallingRockPool::getInstance()->returnEnemy(fallingRock);
    }
    else if (auto randomBoom = dynamic_cast<RandomBoom*>(enemy)) {
        RandomBoomPool::getInstance()->returnEnemy(randomBoom);
    }
}

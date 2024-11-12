#include "EnemyPlaneBulletPool.h"
#include "EnemyPlaneBullet.h"
#include "cocos2d.h"

USING_NS_CC;

EnemyPlaneBulletPool* EnemyPlaneBulletPool::getInstance() {
    static EnemyPlaneBulletPool instance;
    return &instance;
}

void EnemyPlaneBulletPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        EnemyPlaneBullet* enemy = EnemyPlaneBullet::createEnemyBullet();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            _availableEnemies.push(enemy);
        }
    }
}

EnemyPlaneBullet* EnemyPlaneBulletPool::getEnemy() {
    if (_availableEnemies.empty()) {
        EnemyPlaneBullet* enemy = EnemyPlaneBullet::createEnemyBullet();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            return enemy;
        }
        return nullptr;
    }
    EnemyPlaneBullet* enemy = _availableEnemies.front();
    _availableEnemies.pop();
    return enemy;
}

void EnemyPlaneBulletPool::returnEnemy(EnemyPlaneBullet* enemy) {
    if (enemy) {
        enemy->reset();
        _availableEnemies.push(enemy);
    }
}

void EnemyPlaneBulletPool::resetPool() {
    while (!_availableEnemies.empty()) {
        EnemyPlaneBullet* enemy = _availableEnemies.front();
        _availableEnemies.pop();
        enemy->release();
    }
}

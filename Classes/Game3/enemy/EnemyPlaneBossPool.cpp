#include "EnemyPlaneBossPool.h"
#include "EnemyPlaneBoss.h"
#include "cocos2d.h"

USING_NS_CC;

EnemyPlaneBossPool* EnemyPlaneBossPool::getInstance() {
    static EnemyPlaneBossPool instance;
    return &instance;
}

void EnemyPlaneBossPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        EnemyPlaneBoss* enemy = EnemyPlaneBoss::createEnemyPlaneBoss();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            _availableEnemies.push(enemy);
        }
    }
}

EnemyPlaneBoss* EnemyPlaneBossPool::getEnemy() {
    if (_availableEnemies.empty()) {
        return nullptr;
    }
    EnemyPlaneBoss* enemy = _availableEnemies.front();
    _availableEnemies.pop();
    return enemy;
}

void EnemyPlaneBossPool::returnEnemy(EnemyPlaneBoss* enemy) {
    if (enemy) {
        enemy->reset();
        _availableEnemies.push(enemy);
    }
}

void EnemyPlaneBossPool::resetPool() {
    while (!_availableEnemies.empty()) {
        EnemyPlaneBoss* enemy = _availableEnemies.front();
        _availableEnemies.pop();
        enemy->release();
    }
}

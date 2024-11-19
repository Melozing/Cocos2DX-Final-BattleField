#include "EnemyPlaneBoomPool.h"
#include "EnemyPlaneBoom.h"
#include "cocos2d.h"

USING_NS_CC;

EnemyPlaneBoomPool* EnemyPlaneBoomPool::getInstance() {
    static EnemyPlaneBoomPool instance;
    return &instance;
}

void EnemyPlaneBoomPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        EnemyPlaneBoom* enemy = EnemyPlaneBoom::createEnemyPlaneBoom();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            _availableEnemies.push(enemy);
        }
    }
}

EnemyPlaneBoom* EnemyPlaneBoomPool::getEnemy() {
    if (_availableEnemies.empty()) {
        EnemyPlaneBoom* enemy = EnemyPlaneBoom::createEnemyPlaneBoom();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            return enemy;
        }
        return nullptr;
    }
    EnemyPlaneBoom* enemy = _availableEnemies.front();
    _availableEnemies.pop();
    return enemy;
}

void EnemyPlaneBoomPool::returnEnemy(EnemyPlaneBoom* enemy) {
    if (enemy) {
        enemy->reset();
        _availableEnemies.push(enemy);
    }
}

void EnemyPlaneBoomPool::resetPool() {
    while (!_availableEnemies.empty()) {
        EnemyPlaneBoom* enemy = _availableEnemies.front();
        _availableEnemies.pop();
        enemy->release();
    }
}

#include "NewEnemyPool.h"

NewEnemyPool* NewEnemyPool::getInstance() {
    static NewEnemyPool instance;
    return &instance;
}

void NewEnemyPool::initPool(int size) {
    for (int i = 0; i < size; ++i) {
        auto enemy = NewEnemy::create();
        enemy->retain();
        _pool.push_back(enemy);
    }
}

NewEnemy* NewEnemyPool::getEnemy() {
    if (!_pool.empty()) {
        auto enemy = _pool.back();
        _pool.pop_back();
        return enemy;
    }
    return nullptr;
}

void NewEnemyPool::returnEnemy(NewEnemy* enemy) {
    _pool.push_back(enemy);
}

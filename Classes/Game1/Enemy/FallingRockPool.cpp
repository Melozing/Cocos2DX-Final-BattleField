#include "FallingRockPool.h"

USING_NS_CC;

FallingRockPool* FallingRockPool::getInstance() {
    static FallingRockPool instance;
    return &instance;
}

void FallingRockPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        FallingRock* enemy = FallingRock::create();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            _availableEnemies.push(enemy);
        }
    }
}

FallingRock* FallingRockPool::getEnemy() {
    if (_availableEnemies.empty()) {
        return nullptr;
    }
    FallingRock* enemy = _availableEnemies.front();
    _availableEnemies.pop();
    return enemy;
}

void FallingRockPool::returnEnemy(FallingRock* enemy) {
    if (enemy) {
        enemy->reset();
        _availableEnemies.push(enemy);
    }
}

#include "FlyingBulletPool.h"
#include "cocos2d.h"

USING_NS_CC;

FlyingBulletPool* FlyingBulletPool::getInstance() {
    static FlyingBulletPool instance;
    return &instance;
}

void FlyingBulletPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        FlyingBullet* enemy = FlyingBullet::create();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            _availableEnemies.push(enemy);
        }
    }
}

FlyingBullet* FlyingBulletPool::getEnemy() {
    if (_availableEnemies.empty()) {
        FlyingBullet* enemy = FlyingBullet::create();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            return enemy;
        }
        return nullptr;
    }
    FlyingBullet* enemy = _availableEnemies.front();
    _availableEnemies.pop();
    return enemy;
}

void FlyingBulletPool::returnEnemy(FlyingBullet* enemy) {
    if (enemy) {
        enemy->reset();
        _availableEnemies.push(enemy);
    }
}

void FlyingBulletPool::resetPool() {
    while (!_availableEnemies.empty()) {
        FlyingBullet* enemy = _availableEnemies.front();
        _availableEnemies.pop();
        enemy->release();
    }
}

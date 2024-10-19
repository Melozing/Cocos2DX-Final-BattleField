#include "FanBulletPool.h"
#include "FanBullet.h"
#include "cocos2d.h"

USING_NS_CC;

FanBulletPool* FanBulletPool::getInstance() {
    static FanBulletPool instance;
    return &instance;
}

void FanBulletPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        FanBullet* enemy = FanBullet::create();
        if (enemy) {
            enemy->retain();
            enemy->reset();
            _availableEnemies.push(enemy);
        }
    }
}

FanBullet* FanBulletPool::getEnemy() {
    if (_availableEnemies.empty()) {
        return nullptr;
    }
    FanBullet* enemy = _availableEnemies.front();
    _availableEnemies.pop();
    return enemy;
}

void FanBulletPool::returnEnemy(FanBullet* enemy) {
    if (enemy) {
        enemy->reset();
        _availableEnemies.push(enemy);
    }
}

void FanBulletPool::resetPool() {
    while (!_availableEnemies.empty()) {
        FanBullet* enemy = _availableEnemies.front();
        _availableEnemies.pop();
        enemy->release();
    }
}
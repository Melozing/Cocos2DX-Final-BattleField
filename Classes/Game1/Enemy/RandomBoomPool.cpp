#include "RandomBoomPool.h"
#include "cocos2d.h"

USING_NS_CC;

RandomBoomPool* RandomBoomPool::getInstance() {
    static RandomBoomPool instance;
    return &instance;
}

void RandomBoomPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        RandomBoom* enemy = RandomBoom::create();
        if (enemy) {
            enemy->retain();
            enemy->setVisible(false);
            _availableEnemies.push(enemy);
        }
    }
}

RandomBoom* RandomBoomPool::getEnemy() {
    if (_availableEnemies.empty()) {
        RandomBoom* enemy = RandomBoom::create();
        if (enemy) {
            enemy->retain();
            enemy->setVisible(true);
            return enemy;
        }
        return nullptr;
    }
    RandomBoom* enemy = _availableEnemies.front();
    _availableEnemies.pop();
    enemy->setVisible(true);
    return enemy;
}

void RandomBoomPool::returnEnemy(RandomBoom* enemy) {
    if (enemy) {
        enemy->setVisible(false);
        enemy->setPosition(Vec2(-100, -100));
        _availableEnemies.push(enemy);
    }
}

void RandomBoomPool::resetPool() {
    while (!_availableEnemies.empty()) {
        RandomBoom* enemy = _availableEnemies.front();
        _availableEnemies.pop();
        enemy->release();
    }
}

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
    CCLOG("RandomBoomPool initialized with %d enemies", poolSize);
}

RandomBoom* RandomBoomPool::getEnemy() {
    if (_availableEnemies.empty()) {
        RandomBoom* enemy = RandomBoom::create();
        if (enemy) {
            enemy->retain();
            enemy->setVisible(true);
            CCLOG("RandomBoom created and retrieved. No enemies in pool.");
            return enemy;
        }
        return nullptr;
    }
    RandomBoom* enemy = _availableEnemies.front();
    _availableEnemies.pop();
    enemy->setVisible(true);
    CCLOG("RandomBoom retrieved from pool. Remaining enemies: %d", _availableEnemies.size());
    return enemy;
}

void RandomBoomPool::returnEnemy(RandomBoom* enemy) {
    if (enemy) {
        enemy->setVisible(false);
        enemy->setPosition(Vec2(-100, -100));
        _availableEnemies.push(enemy);
        CCLOG("RandomBoom returned to pool. Available enemies: %d", _availableEnemies.size());
    }
}

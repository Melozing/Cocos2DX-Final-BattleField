#include "EnemyPool.h"
#include "EnemyFactory.h"

USING_NS_CC;

EnemyPool* EnemyPool::getInstance() {
    static EnemyPool instance;
    return &instance;
}

void EnemyPool::initPool(const std::string& enemyType, int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        Enemy* enemy = EnemyFactory::spawnEnemy(enemyType, Vec2(-100, -100));
        enemy->setVisible(false);
        _availableEnemies.push_back(enemy);
    }
}

Enemy* EnemyPool::getEnemy() {
    if (_availableEnemies.empty()) {
        return nullptr; 
    }
    Enemy* enemy = _availableEnemies.back();
    _availableEnemies.pop_back();
    _activeEnemies.push_back(enemy);
    enemy->setVisible(true);
    return enemy;
}

void EnemyPool::returnEnemy(Enemy* enemy) {
    enemy->setVisible(false);
    enemy->setPosition(Vec2(-100, -100)); 
    _activeEnemies.erase(std::remove(_activeEnemies.begin(), _activeEnemies.end(), enemy), _activeEnemies.end());
    _availableEnemies.push_back(enemy);
}

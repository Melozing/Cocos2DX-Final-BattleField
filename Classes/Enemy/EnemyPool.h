#ifndef __ENEMY_POOL_H__
#define __ENEMY_POOL_H__

#include "cocos2d.h"
#include "Enemy.h"
#include <vector>

class EnemyPool {
public:
    static EnemyPool* getInstance();

    void initPool(const std::string& enemyType, int poolSize);

    Enemy* getEnemy();

    void returnEnemy(Enemy* enemy);

private:
    std::vector<Enemy*> _availableEnemies;
    std::vector<Enemy*> _activeEnemies;
};

#endif // __ENEMY_POOL_H__

#ifndef ENEMYPLANE_BOSS_POOL_H
#define ENEMYPLANE_BOSS_POOL_H

#include "cocos2d.h"
#include <queue>

class EnemyPlaneBoss;

class EnemyPlaneBossPool {
public:
    static EnemyPlaneBossPool* getInstance();
    void initPool(int poolSize);
    EnemyPlaneBoss* getEnemy();
    void returnEnemy(EnemyPlaneBoss* enemy);
    void resetPool();
private:
    std::queue<EnemyPlaneBoss*> _availableEnemies;
};

#endif // ENEMYPLANE_BOSS_POOL_H

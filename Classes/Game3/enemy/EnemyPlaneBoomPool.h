#ifndef ENEMYPLANE_BOOM_POOL_H
#define ENEMYPLANE_BOOM_POOL_H

#include "cocos2d.h"
#include <queue>

class EnemyPlaneBoom;

class EnemyPlaneBoomPool {
public:
    static EnemyPlaneBoomPool* getInstance();
    void initPool(int poolSize);
    EnemyPlaneBoom* getEnemy();
    void returnEnemy(EnemyPlaneBoom* enemy);
    void resetPool();
private:
    std::queue<EnemyPlaneBoom*> _availableEnemies;
};

#endif // ENEMYPLANE_BOOM_POOL_H

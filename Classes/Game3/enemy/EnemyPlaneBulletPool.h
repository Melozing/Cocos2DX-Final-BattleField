#ifndef ENEMYPLANE_BULLET_POOL_H
#define ENEMYPLANE_BULLET_POOL_H

#include "cocos2d.h"
#include <queue>

class EnemyPlaneBullet;

class EnemyPlaneBulletPool {
public:
    static EnemyPlaneBulletPool* getInstance();
    void initPool(int poolSize);
    EnemyPlaneBullet* getEnemy();
    void returnEnemy(EnemyPlaneBullet* enemy);
    void resetPool();
private:
    std::queue<EnemyPlaneBullet*> _availableEnemies;
};

#endif // ENEMYPLANE_BULLET_POOL_H

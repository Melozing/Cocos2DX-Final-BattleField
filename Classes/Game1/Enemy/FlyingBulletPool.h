#ifndef __FLYING_BULLET_POOL_H__
#define __FLYING_BULLET_POOL_H__

#include "FlyingBullet.h"
#include <queue>

class FlyingBulletPool {
public:
    static FlyingBulletPool* getInstance();
    void initPool(int poolSize);
    FlyingBullet* getEnemy();
    void returnEnemy(FlyingBullet* enemy);
    void resetPool();
private:
    std::queue<FlyingBullet*> _availableEnemies;
};

#endif // __FLYING_BULLET_POOL_H__

#ifndef __BULLET_FOR_ENEMY_PLANE_POOL_H__
#define __BULLET_FOR_ENEMY_PLANE_POOL_H__

#include "BulletForEnemyPlane.h"
#include <queue>

class BulletForEnemyPlanePool {
public:
    static BulletForEnemyPlanePool* getInstance();
    void initPool(int poolSize);
    BulletForEnemyPlane* getBullet();
    void returnBullet(BulletForEnemyPlane* bullet);
    void resetPool();
private:
    std::queue<BulletForEnemyPlane*> _availableBullets;
};

#endif // __BULLET_FOR_ENEMY_PLANE_POOL_H__

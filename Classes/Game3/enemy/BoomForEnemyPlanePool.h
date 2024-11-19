#ifndef __BOOM_FOR_ENEMY_PLANE_POOL_H__
#define __BOOM_FOR_ENEMY_PLANE_POOL_H__

#include "BoomForEnemyPlane.h"
#include <queue>

class BoomForEnemyPlanePool {
public:
    static BoomForEnemyPlanePool* getInstance();
    void initPool(int poolSize);
    BoomForEnemyPlane* getBoom();
    void returnBoom(BoomForEnemyPlane* boom);
    void resetPool();
private:
    std::queue<BoomForEnemyPlane*> _availableBooms;
};

#endif // __BOOM_FOR_ENEMY_PLANE_POOL_H__

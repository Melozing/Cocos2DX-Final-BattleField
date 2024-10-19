#ifndef FANBULLETPOOL_H
#define FANBULLETPOOL_H

#include "cocos2d.h"
#include <queue>

class FanBullet;

class FanBulletPool {
public:
    static FanBulletPool* getInstance();
    void initPool(int poolSize);
    FanBullet* getEnemy();
    void returnEnemy(FanBullet* enemy);
    void resetPool();
private:
    std::queue<FanBullet*> _availableEnemies;
};

#endif // FANBULLETPOOL_H

#ifndef BULLETPOOL_H
#define BULLETPOOL_H

#include "cocos2d.h"
#include "Bullet.h"
#include <queue>


class BulletPool {
public:
    static BulletPool* getInstance();
    void initPool(int poolSize);
    Bullet* getBullet();
    void returnBullet(Bullet* bullet);
    void resetPool();

private:
    std::queue<Bullet*> _availableBullets;
};

#endif // BULLETPOOL_H


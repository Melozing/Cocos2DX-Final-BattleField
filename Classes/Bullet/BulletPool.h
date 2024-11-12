#pragma once

#include "Bullet/Bullet.h"
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

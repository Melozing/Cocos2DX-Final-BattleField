// BulletPool.h
#pragma once

#include "Bullet/Bullet.h"
#include <vector>

class BulletPool
{
public:
    BulletPool(size_t size);
    ~BulletPool();

    Bullet* getBullet();
    void returnBullet(Bullet* bullet);
    size_t getRemainingBullets() const;

private:
    std::vector<Bullet*> pool;
    size_t poolSize;
};

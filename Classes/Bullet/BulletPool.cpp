// BulletPool.cpp
#include "Bullet/BulletPool.h"
#include "Bullet/Bullet.h"

BulletPool::BulletPool(size_t size) : poolSize(size)
{
    for (size_t i = 0; i < poolSize; ++i)
    {
        Bullet* bullet = new Bullet();
        bullet->retain(); // Increase reference count
        bullet->deactivate();
        pool.push_back(bullet);
    }
}

BulletPool::~BulletPool()
{
    for (Bullet* bullet : pool)
    {
        bullet->release(); // Decrease reference count
    }
}

Bullet* BulletPool::getBullet()
{
    if (!pool.empty())
    {
        Bullet* bullet = pool.back();
        pool.pop_back();
        bullet->activate();
        return bullet;
    }
    return nullptr; // Or create a new bullet if pool is empty
}

void BulletPool::returnBullet(Bullet* bullet)
{
    bullet->deactivate();
    pool.push_back(bullet);
}

size_t BulletPool::getRemainingBullets() const
{
    return pool.size();
}

// BulletManager.cpp
#include "Bullet/BulletManager.h"
#include "Constants/Constants.h"
#include "cocos2d.h"
#include "Bullet/Bullet.h"
USING_NS_CC;

BulletManager::BulletManager(size_t poolSize, const std::string& bulletImage)
    : poolSize(poolSize), bulletImage(bulletImage)
{
    bulletPool = new BulletPool(poolSize);
    Init();
}

BulletManager::~BulletManager()
{
    delete bulletPool;
}

void BulletManager::Init()
{
    for (size_t i = 0; i < poolSize; ++i)
    {
        Bullet* bullet = Bullet::createBullet(bulletImage, Vec2::ZERO, 0.0f);
        bullet->retain(); // Increase reference count
        bulletPool->returnBullet(bullet);
    }
}

void BulletManager::Update(float delta)
{
    for (auto& bullet : activeBullets)
    {
        bullet->update(delta);
    }
}

void BulletManager::SpawnBullet(const Vec2& position, const Vec2& direction, float speed)
{
    Bullet* bullet = bulletPool->getBullet();
    if (bullet)
    {
        bullet->setPosition(position);
        bullet->setDirection(direction);
        bullet->setSpeed(speed);
        bullet->activate();
        activeBullets.push_back(bullet);
        Director::getInstance()->getRunningScene()->addChild(bullet);
    }
}

void BulletManager::ReturnBullet(Bullet* bullet)
{
    bulletPool->returnBullet(bullet);
    activeBullets.erase(std::remove(activeBullets.begin(), activeBullets.end(), bullet), activeBullets.end());
}

size_t BulletManager::getRemainingBullets() const
{
    return bulletPool->getRemainingBullets();
}

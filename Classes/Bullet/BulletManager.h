// BulletManager.h
#pragma once

#include "Bullet/BulletPool.h"
#include <vector>

class BulletManager
{
public:
    BulletManager(size_t poolSize, const std::string& bulletImage);
    ~BulletManager();

    void Init();
    void Update(float deltaTime);
    void SpawnBullet(const cocos2d::Vec2& position, const cocos2d::Vec2& direction, float speed);
    void ReturnBullet(Bullet* bullet);
    size_t getRemainingBullets() const;

private:
    BulletPool* bulletPool;
    std::vector<Bullet*> activeBullets;
    size_t poolSize;
    std::string bulletImage;
};

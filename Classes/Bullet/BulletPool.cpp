#include "Bullet/BulletPool.h"
#include "Bullet/Bullet.h"
#include "cocos2d.h"

USING_NS_CC;

BulletPool* BulletPool::getInstance() {
    static BulletPool instance;
    return &instance;
}

void BulletPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        Bullet* bullet = Bullet::create();
        if (bullet) {
            bullet->retain();
            bullet->reset();
            _availableBullets.push(bullet);
        }
    }
}

Bullet* BulletPool::getBullet() {
    if (_availableBullets.empty()) {
        Bullet* bullet = Bullet::create();
        if (bullet) {
            bullet->retain();
            bullet->reset();
            return bullet;
        }
        return nullptr;
    }
    Bullet* bullet = _availableBullets.front();
    _availableBullets.pop();
    bullet->setVisible(true);
    return bullet;
}

void BulletPool::returnBullet(Bullet* bullet) {
    if (bullet) {
        bullet->reset();
        bullet->removeFromParentAndCleanup(false);
        _availableBullets.push(bullet);
    }
}

void BulletPool::resetPool() {
    while (!_availableBullets.empty()) {
        Bullet* bullet = _availableBullets.front();
        _availableBullets.pop();
        bullet->release();
    }
}
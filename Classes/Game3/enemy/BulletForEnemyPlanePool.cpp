#include "BulletForEnemyPlanePool.h"
#include "BulletForEnemyPlane.h"

USING_NS_CC;

BulletForEnemyPlanePool* BulletForEnemyPlanePool::getInstance() {
    static BulletForEnemyPlanePool instance;
    return &instance;
}

void BulletForEnemyPlanePool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        BulletForEnemyPlane* bullet = BulletForEnemyPlane::createBullet();
        if (bullet) {
            bullet->retain();
            bullet->reset();
            _availableBullets.push(bullet);
        }
    }
}

BulletForEnemyPlane* BulletForEnemyPlanePool::getBullet() {
    if (_availableBullets.empty()) {
        BulletForEnemyPlane* bullet = BulletForEnemyPlane::createBullet();
        if (bullet) {
            bullet->retain();
            bullet->reset();
            return bullet;
        }
        return nullptr;
    }
    BulletForEnemyPlane* bullet = _availableBullets.front();
    _availableBullets.pop();
    return bullet;
}

void BulletForEnemyPlanePool::returnBullet(BulletForEnemyPlane* bullet) {
    if (bullet) {
        bullet->reset();
        _availableBullets.push(bullet);
    }
}

void BulletForEnemyPlanePool::resetPool() {
    while (!_availableBullets.empty()) {
        BulletForEnemyPlane* bullet = _availableBullets.front();
        _availableBullets.pop();
        bullet->release();
    }
}
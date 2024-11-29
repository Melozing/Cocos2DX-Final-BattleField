#include "BulletPoolPlayerGame3.h"
#include "BulletPlayerGame3.h"
#include "cocos2d.h"

USING_NS_CC;

BulletPoolPlayerGame3* BulletPoolPlayerGame3::getInstance() {
    static BulletPoolPlayerGame3 instance;
    return &instance;
}

void BulletPoolPlayerGame3::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        BulletPlayerGame3* bullet = BulletPlayerGame3::create();
        if (bullet) {
            bullet->retain();
            bullet->reset();
            _availableBullets.push(bullet);
        }
    }
}

BulletPlayerGame3* BulletPoolPlayerGame3::getBullet() {
    if (_availableBullets.empty()) {
        return nullptr;
    }
    BulletPlayerGame3* bullet = _availableBullets.front();
    _availableBullets.pop();
    return bullet;
}

void BulletPoolPlayerGame3::returnBullet(BulletPlayerGame3* bullet) {
    if (bullet) {
        bullet->reset();
        _availableBullets.push(bullet);
    }
}

void BulletPoolPlayerGame3::resetPool() {
    while (!_availableBullets.empty()) {
        BulletPlayerGame3* bullet = _availableBullets.front();
        _availableBullets.pop();
        bullet->release();
    }
}

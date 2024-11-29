#ifndef BULLETPOOL_H
#define BULLETPOOL_H

#include "cocos2d.h"
#include "BulletPlayerGame3.h"
#include <queue>


class BulletPoolPlayerGame3 {
public:
    static BulletPoolPlayerGame3* getInstance();
    void initPool(int poolSize);
    BulletPlayerGame3* getBullet();
    void returnBullet(BulletPlayerGame3* bullet);
    void resetPool();

private:
    std::queue<BulletPlayerGame3*> _availableBullets;
};

#endif // BULLETPOOL_H


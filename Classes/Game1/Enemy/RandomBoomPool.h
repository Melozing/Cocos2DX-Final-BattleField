#ifndef __RANDOM_BOOM_POOL_H__
#define __RANDOM_BOOM_POOL_H__

#include "cocos2d.h"
#include "RandomBoom.h"
#include <queue>

class RandomBoomPool {
public:
    static RandomBoomPool* getInstance();

    void initPool(int poolSize);
    RandomBoom* getEnemy();
    void returnEnemy(RandomBoom* enemy);
    void resetPool();

private:
    std::queue<RandomBoom*> _availableEnemies;
};

#endif // __RANDOM_BOOM_POOL_H__

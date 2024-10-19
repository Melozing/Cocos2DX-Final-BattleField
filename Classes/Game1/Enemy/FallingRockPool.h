#ifndef __FALLING_ROCK_POOL_H__
#define __FALLING_ROCK_POOL_H__

#include "cocos2d.h"
#include "FallingRock.h"
#include <queue>

class FallingRockPool {
public:
    static FallingRockPool* getInstance();

    void initPool(int poolSize);
    FallingRock* getEnemy();
    void returnEnemy(FallingRock* enemy);
    void resetPool();
private:
    std::queue<FallingRock*> _availableEnemies;
};

#endif // __FALLING_ROCK_POOL_H__

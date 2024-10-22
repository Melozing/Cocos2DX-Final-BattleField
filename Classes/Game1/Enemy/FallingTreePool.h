#ifndef __FALLING_TREE_POOL_H__
#define __FALLING_TREE_POOL_H__

#include "cocos2d.h"
#include "FallingTree.h"
#include <queue>

class FallingTreePool {
public:
    static FallingTreePool* getInstance();

    void initPool(int poolSize);
    FallingTree* getEnemy();
    void returnEnemy(FallingTree* tree);
    void resetPool();

private:
    std::queue<FallingTree*> _availableTrees;
};

#endif // __FALLING_TREE_POOL_H__

#ifndef __NEW_ENEMY_POOL_H__
#define __NEW_ENEMY_POOL_H__

#include "NewEnemy.h"
#include <vector>

class NewEnemyPool {
public:
    static NewEnemyPool* getInstance();
    void initPool(int size);
    NewEnemy* getEnemy();
    void returnEnemy(NewEnemy* enemy);

private:
    std::vector<NewEnemy*> _pool;
};

#endif // __NEW_ENEMY_POOL_H__

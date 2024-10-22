#ifndef __HEALTH_ITEM_POOL_H__
#define __HEALTH_ITEM_POOL_H__

#include "HealthItem.h"
#include "cocos2d.h"
#include <queue>

class HealthItemPool {
public:
    static HealthItemPool* getInstance();
    void initPool(int size);
    HealthItem* getItem();
    void returnItem(HealthItem* item);
    void resetPool();
private:
    HealthItemPool() = default;
    std::queue<HealthItem*> _availableItems;
};

#endif // __HEALTH_ITEM_POOL_H__

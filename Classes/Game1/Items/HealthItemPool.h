#ifndef __HEALTH_ITEM_POOL_H__
#define __HEALTH_ITEM_POOL_H__

#include "HealthItem.h"
#include "cocos2d.h"

class HealthItemPool {
public:
    static HealthItemPool* getInstance();
    void initPool(int size);
    HealthItem* getItem();
    void returnItem(HealthItem* item);

private:
    HealthItemPool() = default;
    cocos2d::Vector<HealthItem*> _pool;
};

#endif // __HEALTH_ITEM_POOL_H__

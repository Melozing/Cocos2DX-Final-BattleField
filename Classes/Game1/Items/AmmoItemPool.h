#ifndef __AMMO_ITEM_POOL_H__
#define __AMMO_ITEM_POOL_H__

#include "AmmoItem.h"
#include "cocos2d.h"
#include <queue>

class AmmoItemPool {
public:
    static AmmoItemPool* getInstance();
    void initPool(int size);
    AmmoItem* getItem();
    void returnItem(AmmoItem* item);
    void resetPool();
private:
    AmmoItemPool() = default;
    std::queue<AmmoItem*> _availableItems;
};

#endif // __AMMO_ITEM_POOL_H__

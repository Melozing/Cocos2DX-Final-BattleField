#ifndef __AMMO_ITEM_POOL_H__
#define __AMMO_ITEM_POOL_H__

#include "AmmoItem.h"
#include "cocos2d.h"

class AmmoItemPool {
public:
    static AmmoItemPool* getInstance();
    void initPool(int size);
    AmmoItem* getItem();
    void returnItem(AmmoItem* item);

private:
    AmmoItemPool() = default;
    cocos2d::Vector<AmmoItem*> _pool;
};

#endif // __AMMO_ITEM_POOL_H__

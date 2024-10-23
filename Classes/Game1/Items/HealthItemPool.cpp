// HealthItemPool.cpp
#include "HealthItemPool.h"
#include "cocos2d.h"

USING_NS_CC;

HealthItemPool* HealthItemPool::getInstance() {
    static HealthItemPool instance;
    return &instance;
}

void HealthItemPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        auto item = HealthItem::create();
        if (item) {
            item->retain();
            item->reset();
            _availableItems.push(item);
        }
    }
    CCLOG("HealthItemPool initialized with %d items.", poolSize);
}

HealthItem* HealthItemPool::getItem() {
    if (_availableItems.empty()) {
        auto item = HealthItem::create();
        if (item) {
            item->retain();
            item->reset();
            CCLOG("HealthItemPool: No available items. Created a new item. Pool size: %d", _availableItems.size());
            return item;
        }
        return nullptr;
    }
    HealthItem* item = _availableItems.front();
    _availableItems.pop();
    CCLOG("HealthItemPool: Retrieved an item. Pool size: %d", _availableItems.size());
    return item;
}

void HealthItemPool::returnItem(HealthItem* item) {
    if (item) {
        item->reset();
        _availableItems.push(item);
        CCLOG("HealthItemPool: Returned an item. Pool size: %d", _availableItems.size());
    }
}

void HealthItemPool::resetPool() {
    while (!_availableItems.empty()) {
        auto item = _availableItems.front();
        _availableItems.pop();
        item->release();
    }
    CCLOG("HealthItemPool: Pool reset. Pool size: %d", _availableItems.size());
}

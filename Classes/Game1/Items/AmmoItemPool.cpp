// AmmoItemPool.cpp
#include "AmmoItemPool.h"
#include "cocos2d.h"

USING_NS_CC;

AmmoItemPool* AmmoItemPool::getInstance() {
    static AmmoItemPool instance;
    return &instance;
}

void AmmoItemPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        auto item = AmmoItem::create();
        if (item) {
            item->retain();
            item->reset();
            _availableItems.push(item);
        }
    }
    CCLOG("AmmoItemPool initialized with %d items.", poolSize);
}

AmmoItem* AmmoItemPool::getItem() {
    if (_availableItems.empty()) {
        auto item = AmmoItem::create();
        if (item) {
            item->retain();
            item->reset();
            CCLOG("AmmoItemPool: No available items. Created a new item. Pool size: %d", _availableItems.size());
            return item;
        }
        return nullptr;
    }
    AmmoItem* item = _availableItems.front();
    _availableItems.pop();
    CCLOG("AmmoItemPool: Retrieved an item. Pool size: %d", _availableItems.size());
    return item;
}

void AmmoItemPool::returnItem(AmmoItem* item) {
    if (item) {
        item->reset();
        _availableItems.push(item);
        CCLOG("AmmoItemPool: Returned an item. Pool size: %d", _availableItems.size());
    }
}

void AmmoItemPool::resetPool() {
    while (!_availableItems.empty()) {
        auto item = _availableItems.front();
        _availableItems.pop();
        item->release();
    }
    CCLOG("AmmoItemPool: Pool reset. Pool size: %d", _availableItems.size());
}

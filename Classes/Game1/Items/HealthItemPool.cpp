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
}

HealthItem* HealthItemPool::getItem() {
    if (_availableItems.empty()) {
        auto item = HealthItem::create();
        if (item) {
            item->retain();
            item->reset();
            return item;
        }
        return nullptr;
    }
    HealthItem* item = _availableItems.front();
    _availableItems.pop();
    return item;
}

void HealthItemPool::returnItem(HealthItem* item) {
    if (item) {
        item->reset();
        _availableItems.push(item);
    }
}

void HealthItemPool::resetPool() {
    while (!_availableItems.empty()) {
        auto item = _availableItems.front();
        _availableItems.pop();
        item->release();
    }
}

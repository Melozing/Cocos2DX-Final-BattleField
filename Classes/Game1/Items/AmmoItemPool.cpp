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
}

AmmoItem* AmmoItemPool::getItem() {
    if (_availableItems.empty()) {
        auto item = AmmoItem::create();
        if (item) {
            item->retain();
            item->reset();
            return item;
        }
        return nullptr;
    }
    AmmoItem* item = _availableItems.front();
    _availableItems.pop();
    return item;
}

void AmmoItemPool::returnItem(AmmoItem* item) {
    if (item) {
        item->reset();
        _availableItems.push(item);
    }
}

void AmmoItemPool::resetPool() {
    while (!_availableItems.empty()) {
        auto item = _availableItems.front();
        _availableItems.pop();
        item->release();
    }
}

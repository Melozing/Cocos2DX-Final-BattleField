#include "AmmoItemPool.h"
#include "AmmoItem.h"
#include <queue>

AmmoItemPool* AmmoItemPool::getInstance() {
    static AmmoItemPool instance;
    return &instance;
}

void AmmoItemPool::initPool(int size) {
    for (int i = 0; i < size; ++i) {
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
    else {
        auto item = _availableItems.front();
        _availableItems.pop();
        return item;
    }
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

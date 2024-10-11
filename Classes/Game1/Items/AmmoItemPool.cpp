#include "AmmoItemPool.h"

AmmoItemPool* AmmoItemPool::getInstance() {
    static AmmoItemPool instance;
    return &instance;
}

void AmmoItemPool::initPool(int size) {
    for (int i = 0; i < size; ++i) {
        auto item = AmmoItem::create();
        item->retain();
        _pool.pushBack(item);
    }
}

AmmoItem* AmmoItemPool::getItem() {
    if (_pool.empty()) {
        return AmmoItem::create();
    }
    else {
        auto item = _pool.back();
        _pool.popBack();
        return item;
    }
}

void AmmoItemPool::returnItem(AmmoItem* item) {
    item->removeFromParent();
    _pool.pushBack(item);
}

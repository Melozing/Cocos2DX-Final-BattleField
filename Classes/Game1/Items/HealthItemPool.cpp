#include "HealthItemPool.h"

HealthItemPool* HealthItemPool::getInstance() {
    static HealthItemPool instance;
    return &instance;
}

void HealthItemPool::initPool(int size) {
    for (int i = 0; i < size; ++i) {
        auto item = HealthItem::create();
        item->retain();
        _pool.pushBack(item);
    }
}

HealthItem* HealthItemPool::getItem() {
    if (_pool.empty()) {
        return HealthItem::create();
    }
    else {
        auto item = _pool.back();
        _pool.popBack();
        return item;
    }
}

void HealthItemPool::returnItem(HealthItem* item) {
    item->removeFromParent();
    _pool.pushBack(item);
}

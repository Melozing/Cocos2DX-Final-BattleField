#include "ShieldSkillItemPool.h"

ShieldSkillItemPool* ShieldSkillItemPool::getInstance() {
    static ShieldSkillItemPool instance;
    return &instance;
}

void ShieldSkillItemPool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        auto item = ShieldSkillItem::create();
        if (item) {
            item->retain();
            item->reset();
            _availableItems.push(item);
        }
    }
}

ShieldSkillItem* ShieldSkillItemPool::getItem() {
    if (_availableItems.empty()) {
        auto item = ShieldSkillItem::create();
        if (item) {
            item->retain();
            item->reset();
            return item;
        }
        return nullptr;
    }
    ShieldSkillItem* item = _availableItems.front();
    _availableItems.pop();
    return item;
}

void ShieldSkillItemPool::returnItem(ShieldSkillItem* item) {
    if (item) {
        item->reset();
        _availableItems.push(item);
    }
}

void ShieldSkillItemPool::resetPool() {
    while (!_availableItems.empty()) {
        auto item = _availableItems.front();
        _availableItems.pop();
        item->release();
    }
}

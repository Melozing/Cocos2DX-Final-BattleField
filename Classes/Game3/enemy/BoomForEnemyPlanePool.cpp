#include "BoomForEnemyPlanePool.h"
#include "BoomForEnemyPlane.h"

USING_NS_CC;

BoomForEnemyPlanePool* BoomForEnemyPlanePool::getInstance() {
    static BoomForEnemyPlanePool instance;
    return &instance;
}

void BoomForEnemyPlanePool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        BoomForEnemyPlane* boom = BoomForEnemyPlane::createBoom();
        if (boom) {
            boom->retain();
            boom->reset();
            _availableBooms.push(boom);
        }
    }
}

BoomForEnemyPlane* BoomForEnemyPlanePool::getBoom() {
    if (_availableBooms.empty()) {
        BoomForEnemyPlane* boom = BoomForEnemyPlane::createBoom();
        if (boom) {
            boom->retain();
            boom->reset();
            return boom;
        }
        return nullptr;
    }
    BoomForEnemyPlane* boom = _availableBooms.front();
    _availableBooms.pop();
    return boom;
}

void BoomForEnemyPlanePool::returnBoom(BoomForEnemyPlane* boom) {
    if (boom) {
        boom->reset();
        _availableBooms.push(boom);
    }
}

void BoomForEnemyPlanePool::resetPool() {
    while (!_availableBooms.empty()) {
        BoomForEnemyPlane* boom = _availableBooms.front();
        _availableBooms.pop();
        boom->release();
    }
}
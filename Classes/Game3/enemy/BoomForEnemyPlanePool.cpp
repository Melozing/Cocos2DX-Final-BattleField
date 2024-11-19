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
    CCLOG("BoomPool initialized with %d booms", poolSize);
}

BoomForEnemyPlane* BoomForEnemyPlanePool::getBoom() {
    if (_availableBooms.empty()) {
        BoomForEnemyPlane* boom = BoomForEnemyPlane::createBoom();
        if (boom) {
            boom->retain();
            boom->reset();
            CCLOG("BoomPool empty. Created new boom. Current pool size: %lu", _availableBooms.size());
            return boom;
        }
        return nullptr;
    }
    BoomForEnemyPlane* boom = _availableBooms.front();
    _availableBooms.pop();
    CCLOG("Boom retrieved from pool. Current pool size: %lu", _availableBooms.size());
    return boom;
}

void BoomForEnemyPlanePool::returnBoom(BoomForEnemyPlane* boom) {
    if (boom) {
        boom->reset();
        _availableBooms.push(boom);
        CCLOG("Boom returned to pool. Current pool size: %lu", _availableBooms.size());
    }
}

void BoomForEnemyPlanePool::resetPool() {
    while (!_availableBooms.empty()) {
        BoomForEnemyPlane* boom = _availableBooms.front();
        _availableBooms.pop();
        boom->release();
    }
    CCLOG("BoomPool reset. All booms released.");
}


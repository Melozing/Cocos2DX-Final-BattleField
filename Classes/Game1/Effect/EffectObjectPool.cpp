#include "EffectObjectPool.h"

EffectObjectPool* EffectObjectPool::getInstance() {
    static EffectObjectPool instance;
    return &instance;
}

void EffectObjectPool::initPool(size_t size, const std::string& filename, const std::string& plistFile) {
    this->filename = filename;
    this->plistFile = plistFile;
    for (size_t i = 0; i < size; ++i) {
        EffectObject* effectObject = EffectObject::create(plistFile, filename);
        effectObject->retain();
        effectObject->setVisible(false);
        pool.push(effectObject);
    }
}

EffectObject* EffectObjectPool::getEffectObject() {
    if (pool.empty()) {
        EffectObject* effectObject = EffectObject::create(plistFile, filename);
        if (effectObject) {
            effectObject->retain();
            effectObject->setVisible(true);
            return effectObject;
        }
        return nullptr;
    }
    EffectObject* effectObject = pool.front();
    pool.pop();
    effectObject->setVisible(true);
    return effectObject;
}

void EffectObjectPool::returnEffectObject(EffectObject* effectObject) {
    if (effectObject) {
        effectObject->reset();
        pool.push(effectObject);
    }
}

void EffectObjectPool::clearPool() {
    while (!pool.empty()) {
        EffectObject* effectObject = pool.front();
        pool.pop();
        effectObject->release();
    }
}

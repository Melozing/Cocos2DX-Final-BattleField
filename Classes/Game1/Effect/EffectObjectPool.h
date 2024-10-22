#ifndef EFFECT_OBJECT_POOL_H
#define EFFECT_OBJECT_POOL_H

#include "EffectObject.h"
#include <queue> 

class EffectObjectPool {
public:
    static EffectObjectPool* getInstance();

    void initPool(size_t size, const std::string& filename, const std::string& plistFile);
    EffectObject* getEffectObject();
    void returnEffectObject(EffectObject* effectObject);
    void clearPool();

private:
    std::queue<EffectObject*> pool;
    std::string filename;
    std::string plistFile;
};

#endif // EFFECT_OBJECT_POOL_H

// PoolBulletGame2.h
#pragma once

#include "Grenade/BulletGame2.h"
#include <vector>
#include <memory>

class PoolBulletGame2
{
public:
    PoolBulletGame2(int initialSize);
    ~PoolBulletGame2();

    std::shared_ptr<BulletGame2> PopElement();
    void AddElement(std::shared_ptr<BulletGame2> element);

private:
    std::vector<std::shared_ptr<BulletGame2>> _pool;
};

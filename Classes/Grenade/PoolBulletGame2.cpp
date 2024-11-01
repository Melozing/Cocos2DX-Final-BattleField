// PoolBulletGame2.cpp
#include "Grenade/PoolBulletGame2.h"

PoolBulletGame2::PoolBulletGame2(int initialSize)
{
    for (int i = 0; i < initialSize; ++i)
    {
        auto bullet = std::make_shared<BulletGame2>();
        _pool.push_back(bullet);
    }
}

PoolBulletGame2::~PoolBulletGame2()
{
    _pool.clear();
}

std::shared_ptr<BulletGame2> PoolBulletGame2::PopElement()
{
    if (!_pool.empty())
    {
        auto element = _pool.back();
        _pool.pop_back();
        return element;
    }
    return nullptr;
}

void PoolBulletGame2::AddElement(std::shared_ptr<BulletGame2> element)
{
    _pool.push_back(element);
}

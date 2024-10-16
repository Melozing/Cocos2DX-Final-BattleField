#include "Pool.h"

template <class T>
Pool<T>::Pool()
{
}
template <class T>
Pool<T>::~Pool()
{
}

template <class T>
size_t Pool<T>::getCount() const
{
    return poolOf.size();
}

template <class T>
T Pool<T>::PopElement()
{
    if (poolOf.empty())
    {
        throw std::out_of_range("Pool is empty");
    }
    T elem = poolOf.back();
    poolOf.pop_back();
    return elem;
}

template <class T>
void Pool<T>::AddElement(T element)
{
    poolOf.push_back(element);
}

//template class Pool<std::shared_ptr<Projectile>>;
//template class Pool<std::shared_ptr<ExplosionBullet>>;
//template class Pool<std::shared_ptr<Enemy>>;
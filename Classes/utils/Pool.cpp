// Pool.cpp
#include "Pool.h"
#include "Grenade/BulletGame2.h"

// Định nghĩa các hàm template
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
        return nullptr; // Trả về nullptr nếu pool trống
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

// Khai báo các template đặc biệt
template class Pool<std::shared_ptr<BulletGame2>>;

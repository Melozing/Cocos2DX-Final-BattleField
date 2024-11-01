#pragma once
#include <vector>
#include <memory>

template <class T>
class Pool
{
public:
    Pool();
    ~Pool();
    size_t getCount() const;
    T PopElement();
    void AddElement(T element);

private:
    std::vector<T> poolOf;
};
class BulletGame2;

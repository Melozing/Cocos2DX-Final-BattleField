// Rotator.cpp
#include "utils/Rotator.h"

Rotator::Rotator(const cocos2d::Vec2* pos, const cocos2d::Vec2* dir) : pos(pos), dir(dir) {}

Rotator::Rotator() : pos(nullptr), dir(nullptr) {}

void Rotator::Init(const cocos2d::Vec2* pos, const cocos2d::Vec2* dir)
{
    this->pos = pos;
    this->dir = dir;
}

void Rotator::RotateToDirection(const cocos2d::Vec2& newDir) const
{
    if (pos && dir)
    {
        cocos2d::Vec2 currentDir = *dir;
        MathFunction::RotateToDirection(newDir, *pos, currentDir);
    }
}

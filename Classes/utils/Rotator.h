// Rotator.h
#pragma once
#include "cocos2d.h"
#include "utils/MathFunction.h"

class Rotator
{
public:
    Rotator(const cocos2d::Vec2* pos, const cocos2d::Vec2* dir);
    Rotator();

    void Init(const cocos2d::Vec2* pos, const cocos2d::Vec2* dir);
    void RotateToDirection(const cocos2d::Vec2& newDir) const;

private:
    const cocos2d::Vec2* pos;
    const cocos2d::Vec2* dir;
};

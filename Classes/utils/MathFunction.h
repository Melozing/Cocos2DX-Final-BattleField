// MathFunction.h
#pragma once
#include "cocos2d.h"

struct RotationVar
{
    float OFFSET_SPRITE;
    float ANGLE_SIZE;
};

class MathFunction
{
public:
    static void RotateToDirection(const cocos2d::Vec2& newDirection, const cocos2d::Vec2& pos, cocos2d::Vec2& currentDirection);
    static unsigned int RotateToDirectionFrames(const RotationVar& rVar, const cocos2d::Vec2& dir);
    static float GetDirInDegreesPositive(const cocos2d::Vec2& dir);
};

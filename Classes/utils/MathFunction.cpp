// MathFunction.cpp
#include "utils/MathFunction.h"
#include <cmath>

void MathFunction::RotateToDirection(const cocos2d::Vec2& newDirection, const cocos2d::Vec2& pos, cocos2d::Vec2& currentDirection)
{
    currentDirection.x = newDirection.x - pos.x;
    currentDirection.y = newDirection.y - pos.y;

    currentDirection.normalize();
}

unsigned int MathFunction::RotateToDirectionFrames(const RotationVar& rVar, const cocos2d::Vec2& dir)
{
    return static_cast<unsigned int>(fmod(GetDirInDegreesPositive(dir) + rVar.OFFSET_SPRITE, 360.0f) / rVar.ANGLE_SIZE);
}

float MathFunction::GetDirInDegreesPositive(const cocos2d::Vec2& dir)
{
    float angle = CC_RADIANS_TO_DEGREES(atan2(dir.y, dir.x));
    if (angle < 0)
    {
        angle += 360.0f;
    }
    return angle;
}

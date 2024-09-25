#include "MathFunction.h"
#include <cmath>

float MathFunction::GetDirInDegreesPositive(const cocos2d::Vec2& dir)
{
    float angle = CC_RADIANS_TO_DEGREES(std::atan2(dir.y, dir.x));
    if (angle < 0)
    {
        angle += 360;
    }
    return angle;
}

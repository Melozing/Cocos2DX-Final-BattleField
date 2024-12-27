#ifndef __GRENADE_H__
#define __GRENADE_H__

#include "cocos2d.h"

class Grenade : public cocos2d::Sprite
{
public:
    static Grenade* create();
    virtual bool init() override;
    void setup(const cocos2d::Vec2& startPosition, const cocos2d::Vec2& direction, float throwDuration);
    void explode();
    void createImpactEffect();
    void dealDamage();
    void reset();

private:
    cocos2d::Vec2 _direction;
    float _throwDuration;
    float calculateThrowDistance(float holdTime);
};



#endif // __GRENADE_H__
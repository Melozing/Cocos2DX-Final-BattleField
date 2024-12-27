#ifndef __GROUND_CANNON_GAME2_H__
#define __GROUND_CANNON_GAME2_H__

#include "cocos2d.h"

class GroundCannonGame2 : public cocos2d::Sprite
{
public:
    static GroundCannonGame2* createGround();
    virtual bool init() override;
    CREATE_FUNC(GroundCannonGame2);

private:
    void setupPhysicsBody();
};

#endif // __GROUND_CANNON_GAME2_H__

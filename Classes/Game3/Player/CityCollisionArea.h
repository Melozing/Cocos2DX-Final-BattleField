#ifndef __CITY_COLLISION_AREA_H__
#define __CITY_COLLISION_AREA_H__

#include "cocos2d.h"

class CityCollisionArea : public cocos2d::Node {
public:
    static CityCollisionArea* createCityCollisionArea();
    virtual bool init() override;
    CREATE_FUNC(CityCollisionArea);

    void createPhysicsBody();

private:
    cocos2d::PhysicsBody* physicsBody;
};

#endif // __CITY_COLLISION_AREA_H__

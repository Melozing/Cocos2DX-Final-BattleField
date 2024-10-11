#pragma once
#include "cocos2d.h"

class PhysicsManager {
public:
    static PhysicsManager* getInstance();
    void setPhysicsBody(cocos2d::Node* node, int bitmask, bool isDynamic = false);

private:
    PhysicsManager() = default;
};
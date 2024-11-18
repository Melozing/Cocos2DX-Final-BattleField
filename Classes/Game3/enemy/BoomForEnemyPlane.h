#ifndef __BOOMFORENEMYPLANE_H__
#define __BOOMFORENEMYPLANE_H__

#include "cocos2d.h"

class BoomForEnemyPlane : public cocos2d::Sprite {
public:
    static BoomForEnemyPlane* createBoom();
    void reset();
    virtual bool init() override;
    void moveDown();
};

#endif // __BOOMFORENEMYPLANE_H__

// EnemyPlane1.h
#ifndef __ENEMY_PLANE_1_H__
#define __ENEMY_PLANE_1_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class EnemyPlane1 : public cocos2d::Sprite, public SpriteController
{
public:
    EnemyPlane1();
    virtual ~EnemyPlane1();
    static EnemyPlane1* createEnemyPlane1();
    virtual bool init() override;
    void initAnimation();

    void startMoving(); // New method to start moving

    // Thêm các hàm di chuyển
    void move(const cocos2d::Size& visibleSize);
    void moveRightToLeft(const cocos2d::Size& visibleSize);
    void moveAndReturn(const cocos2d::Size& visibleSize);
    static void spawnEnemyAfterDelay(float delay, cocos2d::Node* parent);

private:
    cocos2d::Sprite* modelCharac;
};

#endif 

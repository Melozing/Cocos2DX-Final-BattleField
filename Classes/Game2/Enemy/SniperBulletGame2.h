#ifndef __SNIPER_BULLET_GAME2_H__
#define __SNIPER_BULLET_GAME2_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

#pragma once

class SniperBulletGame2 : public cocos2d::Sprite, public SpriteController
{
public:
    static SniperBulletGame2* create();
    bool init() override;

    void setDirection(const cocos2d::Vec2& direction);
    void setSpeed(float speed);
    void reset();
    void spawn();
    void removeWhenOutOfScreen();
    void returnPool();
    void explode();
    void hideModelCharac();
    void update(float delta) override;

private:
    cocos2d::Size GetSize(); 

    cocos2d::Vec2 _direction;
    float _speed;
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::Sprite* modelCharac;
    cocos2d::SpriteBatchNode* explosionBatchNode;
    cocos2d::Sprite* explosionSprite;

    void initAnimation();
    void createPhysicsBody();
    int _damage;
};

#endif //__SNIPER_BULLET_GAME2_H__

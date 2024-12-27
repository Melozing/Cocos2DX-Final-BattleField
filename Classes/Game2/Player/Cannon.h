#pragma once

#include "cocos2d.h"

class Cannon : public cocos2d::Sprite
{
public:
    static Cannon* create();
    virtual bool init() override;
    void initAnimation();
    void setupInitialPosition();
    void createPhysicsBody();
    void addHealthBar();
private:
	float health;
    cocos2d::Size GetSize();
    cocos2d::Sprite* modelCharac;
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::SpriteBatchNode* explosionBatchNode;
};
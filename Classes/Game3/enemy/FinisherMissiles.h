#ifndef __FINISHERMISSILES_H__
#define __FINISHERMISSILES_H__

#include "cocos2d.h"

class FinisherMissiles : public cocos2d::Sprite {
public:
    static FinisherMissiles* create();
    virtual bool init() override;
    void moveDown(bool moveLeft);
    void reset();
    void createPhysicsBody();
    void initAnimation();
    void explode();
    void returnToPool();

private:
    cocos2d::Size GetSize();
    cocos2d::Sprite* modelCharac;
    cocos2d::SpriteBatchNode* spriteBatchNode;
};

#endif // __FINISHERMISSILES_H__

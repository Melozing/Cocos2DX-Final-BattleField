// ItemBase.h
#pragma once
#include "cocos2d.h"

class ItemBaseGame3 : public cocos2d::Sprite {
public:
    virtual bool init() override;
    virtual void applyEffect() = 0; // Pure virtual function to apply the item's effect
    void moveDown();
};
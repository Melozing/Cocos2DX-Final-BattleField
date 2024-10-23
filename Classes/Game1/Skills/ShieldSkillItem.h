// ShieldSkillItem.h
#ifndef __SHIELD_SKILL_ITEM_H__
#define __SHIELD_SKILL_ITEM_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class ShieldSkillItem : public cocos2d::Sprite, public SpriteController {
public:
    static ShieldSkillItem* create();
    virtual bool init() override;
    void reset();
    void playAnimation();
    void initAnimation();
    void activate(float duration); 
    void deactivate();
private:
    Sprite* _shieldSprite;
};

#endif // __SHIELD_SKILL_ITEM_H__

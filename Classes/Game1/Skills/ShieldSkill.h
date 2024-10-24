#ifndef __SHIELD_H__
#define __SHIELD_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class ShieldSkill : public cocos2d::Sprite, public SpriteController {
public:
    static ShieldSkill* create();
    virtual bool init() override;
    void activate(float duration);
    void deactivate();
    bool isActive() const;
    void reset();
private:
    void initAnimation();
    bool _isActive = false;
    cocos2d::Sprite* _shieldSprite;
};

#endif // __SHIELD_H__

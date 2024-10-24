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
    bool _isActive = false;
private:
    void initAnimation();
    cocos2d::Sprite* _shieldSprite;
    SpriteBatchNode* _spriteBatchNode;
};

#endif // __SHIELD_H__

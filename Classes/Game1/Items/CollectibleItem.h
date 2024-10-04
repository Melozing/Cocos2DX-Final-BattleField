#ifndef __COLLECTIBLE_ITEM_H__
#define __COLLECTIBLE_ITEM_H__

#include "cocos2d.h"

class CollectibleItem : public cocos2d::Node {
public:
    virtual bool init() override;
    virtual void applyEffect() = 0; // Pure virtual function
    void spawn(const cocos2d::Vec2& position);
    cocos2d::Size GetSize() const;
    void update(float delta) override;
    void removeWhenOutOfScreen();

protected:
    cocos2d::Sprite* _currentSprite;
    float _speed = 150.0f; // Example speed value
};

#endif // __COLLECTIBLE_ITEM_H__

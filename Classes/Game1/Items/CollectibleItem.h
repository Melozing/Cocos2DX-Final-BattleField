#ifndef __COLLECTIBLE_ITEM_H__
#define __COLLECTIBLE_ITEM_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class CollectibleItem : public cocos2d::Node, public SpriteController {
public:
    static CollectibleItem* create(); // Factory method to create an instance
    virtual bool init() override; // Initialize the item
    void spawn(const cocos2d::Vec2& startPosition); // Set the spawn position
    void update(float delta); // Update method for item behavior
    Size GetSize();
    ~CollectibleItem(); // Destructor

private:
    cocos2d::Sprite* _currentSprite; // Sprite representing the item
    float _spriteScale;
    float _speed = 150.0f; // Speed of falling item
    void removeWhenOutOfScreen();
};

#endif // __COLLECTIBLE_ITEM_H__

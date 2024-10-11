#ifndef __FALLING_TREE_H__
#define __FALLING_TREE_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class FallingTree :public cocos2d::Sprite, public SpriteController {
public:
     bool init() ; 
     void spawn(const cocos2d::Vec2& startPosition); 
     void update(float delta); 
};

#endif // __FALLING_TREE_H__

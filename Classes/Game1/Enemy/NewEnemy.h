#ifndef __NEW_ENEMY_H__
#define __NEW_ENEMY_H__

#include "cocos2d.h"

class NewEnemy : public cocos2d::Sprite {
public:
    static NewEnemy* create();
    void spawn(const cocos2d::Vec2& position);
    cocos2d::Size GetSize() const;
};

#endif // __NEW_ENEMY_H__

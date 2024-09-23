#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#include"cocos2d.h";

class Constants {
public:
    // Constants for enemy scale
    static const float ENEMY_SCALE_SMALL;
    static const float ENEMY_SCALE_MEDIUM;
    static const float ENEMY_SCALE_MEDIUM;
    static const float ENEMY_SCALE_LARGE;
    static const float FLYING_BULLET_SPEED;
    // Player
    static const float PLAYER_RESTRICTEDWIDTH;
    static const float PLAYER_RESTRICTEDHEIGHT;
    static const float PLAYER_MOVESPEED;
    // Screen
    static const cocos2d::Size DESIGN_SIZE;
};

#endif // __CONSTANTS_H__

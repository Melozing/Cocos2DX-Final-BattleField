#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#include "cocos2d.h";

class Constants {
public:
    // Constants for enemy scale
    static const float FLYING_BULLET_SPEED;
    static const float FALLINGROCK_START_Y;
    static const float FALLINGROCK_PADDING;
    // PlayerGame1
    static const float PLAYER_RESTRICTEDWIDTH;
    static const float PLAYER_RESTRICTEDHEIGHT;
    static const float PLAYER_MOVESPEED;
    static const int PLAYER_HEALTH;
    static const float PLAYER_HEALTH_PADDING_X;
    static const float PLAYER_HEALTH_PADDING_X_START;
    static const float PLAYER_HEALTH_PADDING_Y;
    // Screen
    static const cocos2d::Size DESIGN_SIZE;
    //game2
    static const float BulletSpeed;
    static const float PlayerSpeed;
    static const float PlayerScale;
    static const int InitialPosX;
    static const int InitialPosY;
    static const float AnimationFrameDelay;

};

#endif // __CONSTANTS_H__
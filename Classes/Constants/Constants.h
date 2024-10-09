#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#include "cocos2d.h"

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
    //Game1
    static const float TIME_TO_WIN;
    static const std::string pathSoundTrackGame1;
    //Game2
    static const float BulletSpeed;
    static const float PlayerSpeed;
    static const float PlayerScale;
    static const int InitialEnemyPosX;
    static const int InitialEnemyPosY;
    static const float EnemySpeed;
    static const float EnemyScale;
    static const float AttackRange;
    static const float AnimationFrameDelay;
    static const int BulletTag;
    static const int EnemyTag;
    static const int InitialPosX;
    static const int InitialPosY;
    //game3
    static const float GroundY;
    static const float BackgroundSpeed ;
    static const float PlayerScale3;
	//VictoryPanel
	static const std::string VICTORY_SOUNDTRACK_PATH;
};

#endif // __CONSTANTS_H__

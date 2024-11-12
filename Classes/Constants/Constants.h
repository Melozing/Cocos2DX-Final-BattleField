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
    // SHIELD GAME 1
	static const float SHIELD_DURATION;
    static const float SHIELD_TIME_OF_EXISTENCE;
	// ITEM 
	static const float ITEM_SCALE_FACTOR;
	static const float ITEM_EFFECT_DURATION;
	static const float ITEM_SIZE_RATIO;
	//EFFECT_GAME1
    static const std::string EFFECT_EXPLOSION_NAME;
    static const float EFFECT_EXPLOSION_PADDING_SCREEN;
    static const float EFFECT_EXPLOSION_PADDING_PLAYER_AREA;
    static const float EFFECT_MIN_Y_DISTANCE;
    static const float EFFECT_MIN_Y_DISTANCE_BETWEEN_CALLS;
	// EnemyGame1
    static const std::string ENEMY_FLYING_BULLET_NAME;
    static const std::string ENEMY_FALLING_ROCK_AND_BOOM_NAME;
    static const std::string ENEMY_RANDOM_BOOM_NAME;
    static const float FANBULLET_OFFSET;
    static const float FALLINGROCK_ITEMS_OFFSET;
    static const float FALLINGROCK_ITEMS_SPEED;
    static const float RANDOMBOOM_RESTRICTEDHEIGHT;
    static const float RANDOMBOOM_RESTRICTEDWIDTH;
    static const float DISTANCE_ENEMY_FALLING;
    // Screen
    static const cocos2d::Size DESIGN_SIZE;
    static const std::string GAME1_SCENE_NAME;
    static const std::string GAME2_SCENE_NAME;
    static const std::string GAME3_SCENE_NAME;
    static const std::string MAINMENU_SCENE_NAME;
    //Game2
    static const float TIME_TO_WIN;
    static const std::string pathSoundTrackGame1;
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
    static const float scaleFactor;
    static const float BulletScale;
    static const int BulletDamage2;
    static const float GrenadeSpeed;
    static const float GrenadeThrowDistance;

    //bossgame2
    static const float BossScale;
    static const float BossSpeed;
    static const int BossTag;
    static const int BossDamage;
    static const int BossHealth;
    static const float BossReloadTime;
    static const int BossDamageIncrease;
    //bitmask game2
    static const uint32_t player2_bitmask;
    static const uint32_t enemy2_bitmask;
    //game3
    static const float GroundY;
    static const float BackgroundSpeed ;
    static const float DISTANCE_FROM_PLAYER_TO_POINTER;
    static const float PlayerScale3;
    static const float PLAYER_SPEED_GAME3;
    static const float BulletGame3Speed;
    static const float EnemyPlaneBulletGame3Speed; //Speed Enemy Plane 3
	static const float EnemyPlaneBoomGame3Speed; //Speed Enemy Plane 1
	static const float SpeedEnemyPlane_BOSS; //Speed Enemy Plane 1
	
	//VictoryPanel
	static const std::string VICTORY_SOUNDTRACK_PATH;
    //UI
    static const int ORDER_LAYER_CURSOR;
    static const int ORDER_LAYER_UI;
    static const int ORDER_LAYER_LAYOUT_UI;
    static const int ORDER_LAYER_CHARACTER;
    static const int ORDER_LAYER_PLAYER;
    static const int ORDER_LAYER_ITEM;
    static const int ORDER_LAYER_BACKGROUND;
    static const float PADDING_VERTICAL_BUTTONS_MAINMENU;
    static const float PADDING_VERTICAL_UI_PANEL;
    static const float PADDING_VERTICAL_UI_EXITBUTTON;
    static const float PADDING_HORIZONTAL_UI_PANEL;
    static const float PADDING_HORIZONTAL_UI_EXITBUTTON;
};

#endif // __CONSTANTS_H__

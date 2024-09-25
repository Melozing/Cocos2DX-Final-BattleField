#include "Constants.h"
#include "cocos2d.h"

// Initialize the scale values
//SCREEN
const cocos2d::Size Constants::DESIGN_SIZE = cocos2d::Size(960, 640);
//ENEMY
const float Constants::FLYING_BULLET_SPEED = 8.0f;
//PlayerGame1
const float Constants::PLAYER_RESTRICTEDWIDTH = 0.285f;
const float Constants::PLAYER_RESTRICTEDHEIGHT = 0.15f;
const float Constants::PLAYER_MOVESPEED = 2.0f;
const int Constants::PLAYER_HEALTH = 3;

//game2
const float Constants::BulletSpeed = 750.f;
const float Constants::PlayerSpeed = 200.0f;
const float Constants::PlayerScale = 0.5f;
const int Constants::InitialPosX = 100;
const int Constants::InitialPosY = 100;
const float Constants::AnimationFrameDelay = 0.1f;
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
public:
    // Create a player instance
    static Player* createPlayer();

    // Initialize the player
    virtual bool init();

    // Update the player position
    void update(float delta);

    // Handle keyboard input
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // Getters for direction and speed
    cocos2d::Vec2 getDirection() const;
    float getSpeed() const;

    // Implement the "static create()" method manually
    CREATE_FUNC(Player);

private:
    // Movement speed
    float speed;

    // Movement direction
    cocos2d::Vec2 direction;
};

#endif // __PLAYER_H__

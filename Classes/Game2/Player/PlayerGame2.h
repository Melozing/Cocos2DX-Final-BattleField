// PlayerGame2.h
#pragma once

#include "cocos2d.h"
#include "Bullet/BulletManager.h"
#include "Controller/SpriteController.h"

class PlayerGame2 : public cocos2d::Sprite, public SpriteController
{
public:
    PlayerGame2(); // Constructor
    virtual ~PlayerGame2(); // Destructor

    static PlayerGame2* createPlayerGame2(); // Factory method
    virtual bool init(); // Initialization method
    void initAnimation(); // Method to initialize animations
    void startMovementAnimation(); // Method to start movement animation
    void onMouseMove(cocos2d::Event* event); // Mouse move event handler
    void onMouseDown(cocos2d::Event* event); // Mouse down event handler
    void onMouseUp(cocos2d::Event* event); // Mouse up event handler
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // Key press event handler
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // Key release event handler
    void moveUp(); // Method to move up
    void moveDown(); // Method to move down
    void moveLeft(); // Method to move left
    void moveRight(); // Method to move right
    void stopVerticalMovement(); // Method to stop vertical movement
    void stopHorizontalMovement(); // Method to stop horizontal movement
    void update(float delta); // Update method
    void RotateToMouse(); // Method to rotate towards the mouse
    void shootBullet(const cocos2d::Vec2& direction); // Method to shoot a bullet
    void die(); // Method to handle player death

private:
    cocos2d::Vec2 _mousePos; // Mouse position
    cocos2d::Vec2 _velocity; // Velocity
    float _speed; // Speed
    bool _isMoving; // Is the player moving
    bool _isMouseDown; // Is the mouse button down
    cocos2d::Sprite* modelCharac; // Character model
    BulletManager* bulletManager; // Bullet manager
};

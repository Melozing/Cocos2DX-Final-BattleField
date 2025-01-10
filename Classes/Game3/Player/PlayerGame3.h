#ifndef __PLAYER_GAME3_H__
#define __PLAYER_GAME3_H__

#include "cocos2d.h"
#include "Manager/PlayerMovementManager.h"
#include "Controller/SpriteController.h"
#include "Bullet/BulletPlayerGame3.h"

// Class declaration
class PlayerGame3 : public cocos2d::Sprite, public SpriteController
{
public:
    // Static method to create an instance of PlayerGame3
    static PlayerGame3* createPlayerGame3();

    // Override init method
    virtual bool init() override;

    // Event handling methods
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);

    // Event handling methods
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    cocos2d::Vec2 _touchPos;

    // Update method
    void update(float delta) override;

    // Physics body creation
    void createPhysicsBody();

    // Bullet count management
    void increaseBulletCount();

    // Move Left and Right
    void startMovingLeft();
    void startMovingRight();
    void stopMoving();

    void setMovementAndShootingDisabled(bool disabled);

private:
    // Helper methods
    cocos2d::Size GetSize();
    void initAnimation();
    void shootBullet(const cocos2d::Vec2& target);
    void updateTurretRotation();
    bool updateDistanceToMouse(const Vec2& position);
    float calculateDistanceToMouse(const Vec2& position);

    // Setup methods
    void setupInitialPosition();
    void setupTurret();
    void setupEventListeners();
    void setupManagers();

    // Member variables
    cocos2d::Sprite* turretSprite; // Turret sprite
    cocos2d::Vec2 _mousePos; // Mouse position
    cocos2d::Sprite* leftWheelSprite;
    cocos2d::Sprite* rightWheelSprite;
    cocos2d::Sprite* handleSprite;
    PlayerMovement* playerMovement; // Player movement manager
    bool isShooting; // Shooting state
    float distanceToMouse; // Distance to mouse
    bool isMouseDown; // Mouse down state
    float shootDelay; // Delay between shots
    float timeSinceLastShot; // Time since last shot
    int bulletCount; // Bullet count
    cocos2d::Vec2 direction; // Shooting direction
    cocos2d::Vec2 turretWorldPos; // Turret world position
    cocos2d::Vec2 localAnchorPoint; // Local anchor point
    cocos2d::Vec2 turretPosition; // Turret position
    bool isMovementAndShootingDisabled;
    float minAngle;
    float maxAngle;

    // Member variables for animations
    cocos2d::SpriteBatchNode* idleBatchNode;
    cocos2d::Sprite* idleSprite;
    void rotateSpritesContinuously(float angle);
    void stopRotatingSprites();
    void setupWheelsAndHandle();
    void updateCursorColor(bool isWithinRange);
    void checkCursorRange();

    // Check Move left & right
    bool isMovingLeft;
    bool isMovingRight;
};

#endif // __PLAYER_GAME3_H__

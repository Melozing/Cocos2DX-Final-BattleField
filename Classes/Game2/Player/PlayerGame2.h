#pragma once

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "Manager/PlayerMovementManager.h"
#include "PlayerAttributes/PlayerAttributes.h"

class PlayerGame2 : public cocos2d::Sprite, public SpriteController {
public:
    // Constructor and Destructor
    PlayerGame2();
    virtual ~PlayerGame2();

    // Create and initialize methods
    static PlayerGame2* createPlayerGame2();
    virtual bool init() override;

    // Animation initialization methods
    void initAnimation();
    void startMovementAnimation();
    Size GetSize();

    // Mouse event handling methods
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);

    // Keyboard event handling methods
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // Update method
    void update(float delta) override;

    // Player control methods
    void RotateToMouse();
    void shootBullet(const cocos2d::Vec2& direction);
    void throwGrenade(const cocos2d::Vec2& direction, float duration);
    void reload();
    void takeDamage(int damage);
    void die();
    void pickUpHealth(int healthAmount);
    void pickUpAmmo(int ammoAmount);
    void pickUpGrenade(int grenadeAmount);
    void createPhysicsBody();

    // Constants
    const int maxMagazineSize = 30;
    const int initialAmmo = 120;
    const int maxGrenades = 5;

    // Sound methods
    void playShootSound();
    void playReloadSound();
    void playGrenadeSound();

private:
    // Member variables for mouse events
    cocos2d::Vec2 _mousePos;
    bool _isMouseDown;
    float _mousePressDuration;
    bool _isThrowingGrenade;
    bool isMouseDown; // Mouse down state
    float shootDelay; // Delay between shots
    float timeSinceLastShot; // Time since last shot
    int bulletCount; // Bullet count
    bool isMovementAndShootingDisabled; // Movement and shooting state

    void setMovementAndShootingDisabled(bool disabled); // Method to disable movement and shooting

    // Member variables for sprites and animations
    cocos2d::Sprite* _reloadSprite;
    cocos2d::Sprite* bodySprite;
    cocos2d::Sprite* handsSprite;
    cocos2d::SpriteBatchNode* bodySpriteBatchNode;
    cocos2d::SpriteBatchNode* handsSpriteBatchNode;

    // Member variables for movement and bullet management
    PlayerMovement* playerMovement;
    int totalAmmo;
    int currentMagazine;
    bool isReloading;
    float reloadTime;
    int currentGrenades;

    // Member variables for ammo display
    cocos2d::Label* _ammoLabel;
    void updateAmmoDisplay();

    // Member variables for player attributes
    PlayerAttributes* attributes;

    // Animation initialization methods
    void initBodyAnimation();
    void initHandsAnimation();
};

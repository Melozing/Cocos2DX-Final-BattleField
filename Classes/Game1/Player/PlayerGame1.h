#ifndef __PLAYERGAME1_H__
#define __PLAYERGAME1_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/GameController.h"
#include "Manager/PlayerMovementManager.h"
#include "Game1/Skills/ShieldSkill.h"

class PlayerGame1 : public cocos2d::Sprite, public SpriteController
{
public:
    // Initialization
    static PlayerGame1* createPlayer();
    virtual bool init() override;
    void initAnimation();

    // Movement and Control
    void setJoystickDirection(const cocos2d::Vec2& direction);
    void disableMovement();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void update(float delta) override;
    void setupMovementBoundaries();
    void setupKeyboardListener();
    void setupPlayerMovement();

    // Health and Damage
    void takeDamage();
    bool canTakeDamage();
    void playDamageEffect();
    void playHealthIncreaseEffect();

    // Shield
    void setShield(ShieldSkill* shield);
    void updateShieldPosition(float dt);
    ShieldSkill* _shield;

    // Physics
    void createPhysicsBody();
    void removePhysicsBody();

    // Other
    Size GetSize();
    void fadeOutAndDisable();
    void moveToCenterAndExit();

private:
    // Health
    int _health = 3;
    float _damageCooldown = 1.0f;
    float _lastDamageTime = 0.0f;

    // Movement boundaries
    float minX, maxX, minY, maxY;

    // Components
    PlayerAttributes* attributes;
    GameController* gameController;
    Sprite* modelCharac;
    PlayerMovement* playerMovement;
    cocos2d::Vec2 _joystickDirection;
};

#endif // __PLAYERGAME1_H__

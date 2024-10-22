#ifndef __PLAYERGAME1_H__
#define __PLAYERGAME1_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/GameController.h"
#include "Manager/PlayerMovementManager.h"

class PlayerGame1 : public cocos2d::Sprite, public SpriteController
{
public:
    static PlayerGame1* createPlayer();

    virtual bool init() override;

    void initAnimation();
    void takeDamage();
    bool canTakeDamage();
    Size GetSize();

    float boundaryPadding;
    void playDamageEffect();
    void playHealthIncreaseEffect();

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void update(float delta);

    float getMinX() const { return minX; }
    float getMaxX() const { return maxX; }
    float getMinY() const { return minY; }
    float getMaxY() const { return maxY; }
private:
    int _health = 3;
    float minX, maxX, minY, maxY;
    float _damageCooldown = 1.0f;
    float _lastDamageTime = 0.0f;
    PlayerAttributes* attributes;   // Pointer to PlayerAttributes object
    GameController* gameController; // Pointer to GameController object
    Sprite* modelCharac;
    PlayerMovement* playerMovement; // Pointer to PlayerMovement object
};

#endif // __PLAYERGAME1_H__

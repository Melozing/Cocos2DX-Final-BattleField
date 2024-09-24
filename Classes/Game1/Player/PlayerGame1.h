#ifndef __PLAYERGAME1_H__
#define __PLAYERGAME1_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/GameController.h"

class PlayerGame1 : public cocos2d::Sprite, public SpriteController
{
public:
    static PlayerGame1* createPlayer();

    virtual bool init() override;

    void initAnimation();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void takeDamage();  
    bool canTakeDamage(); 

    float boundaryPadding;
    void OnCollisionWithEnemy(int enemyDamage);            // Handles collision with enemy

private:
    int _health = 3;  
    float minX, maxX, minY, maxY;
    float _damageCooldown = 1.0f; 
    float _lastDamageTime = 0.0f;  
    PlayerAttributes* attributes;   // Pointer to PlayerAttributes object
    GameController* gameController; // Pointer to GameController object
};

#endif // __PLAYERGAME1_H__

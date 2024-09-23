#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class Player : public cocos2d::Sprite, public SpriteController
{
public:
    static Player* createPlayer();

    virtual bool init() override;

    void initAnimation();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void setBoundary(float minX, float maxX, float minY, float maxY)
    {
        this->minX = minX;
        this->maxX = maxX;
        this->minY = minY;
        this->maxY = maxY;
    }

    void takeDamage();  
    bool canTakeDamage(); 

    float boundaryPadding;

private:
    int _health = 3;  
    float minX, maxX, minY, maxY;
    float _damageCooldown = 1.0f; 
    float _lastDamageTime = 0.0f;  
};

#endif // __PLAYER_H__

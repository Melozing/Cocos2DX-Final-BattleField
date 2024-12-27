#ifndef __BUNKER_GAME_2_H__
#define __BUNKER_GAME_2_H__

#include "cocos2d.h"
#include "Game2/Player/Cannon.h"
#include "Game2/Player/PlayerGame2.h"

class BunkerGame2 : public cocos2d::Sprite {
public:
    static BunkerGame2* create();
    virtual bool init() override;
    void reset();
    void startSpawning(float interval);
    void setTargetCannon(Cannon* cannon);
    void setPlayer(PlayerGame2* player); 

private:
    cocos2d::Size GetSize();
    void spawnEnemy(float dt);
    void createPhysicsBody();
    cocos2d::Sprite* modalSprite;
    Cannon* targetCannon;
    PlayerGame2* targetPlayer;
};

#endif // __BUNKER_GAME_2_H__

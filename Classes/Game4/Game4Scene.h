#ifndef __GAME4_SCENE_H__
#define __GAME4_SCENE_H__

#include "cocos2d.h"
#include "utils/tilemap.h"
#include  "Scene/BaseScene.h"
class Game4Scene :  public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta) override;
    bool isCollidingWithLayer(Player* player, cocos2d::TMXLayer* layer);
    CREATE_FUNC(Game4Scene);
private:
    Player* player;
    cocos2d::TMXTiledMap* map;
};

#endif // __GAME4_SCENE_H__

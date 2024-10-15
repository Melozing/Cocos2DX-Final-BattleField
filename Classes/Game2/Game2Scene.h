#ifndef __GAME2_SCENE_H__
#define __GAME2_SCENE_H__

#include "cocos2d.h"
#include "Cursor/Cursor.h"
#include "Scene/BaseScene.h"

class Game2Scene : public BaseScene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(Game2Scene);


    void scaleTilemap(cocos2d::TMXTiledMap* tilemap);
private:
    Cursor* _cursor;
};

#endif // __GAME2_SCENE_H__

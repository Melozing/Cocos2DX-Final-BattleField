#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "Cursor/Cursor.h"
#include "Scene/BaseScene.h"

class Game2Scene : public BaseScene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(Game2Scene);
private:
    Cursor* _cursor;
};

#endif // __SPLASH_SCENE_H__

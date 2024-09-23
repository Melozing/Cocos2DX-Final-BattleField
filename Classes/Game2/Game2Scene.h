#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "Cursor/Cursor.h"
class Game2Scene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    CREATE_FUNC(Game2Scene);
private:
    Cursor* _cursor;
};

#endif // __SPLASH_SCENE_H__

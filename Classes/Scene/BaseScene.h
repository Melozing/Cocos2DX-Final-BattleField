#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "cocos2d.h"
#include "Game2/Cursor/Cursor.h"
#include "Controller/SpriteController.h"

class BaseScene : public cocos2d::Scene {
public:
    virtual bool init() override;
    void setSceneCreationFunc(const std::function<cocos2d::Scene* ()>& func);
    CREATE_FUNC(BaseScene);
    void hideCursor(Ref* sender);
    void showCursor(Ref* sender);
    void registerNotification();
    void unregisterNotification();
    Cursor* _cursor;

private:
    void onExit() override;
    std::function<cocos2d::Scene* ()> _sceneCreationFunc;
};

#endif // BASE_SCENE_H

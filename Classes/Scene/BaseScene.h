#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "cocos2d.h"
#include "Game2/Cursor/Cursor.h"
#include "Controller/SpriteController.h"

class BaseScene : public cocos2d::Scene {
public:
    // Initialization
    virtual bool init() override;
    CREATE_FUNC(BaseScene);

    // Scene management
    void setSceneCreationFunc(const std::function<cocos2d::Scene* ()>& func);

    // Cursor management
    virtual void hideCursor(Ref* sender);
    virtual void showCursor(Ref* sender);

    // Notification registration
    void registerNotification();

    // Cursor instance
    Cursor* _cursor;

private:
    // Scene creation function
    std::function<cocos2d::Scene* ()> _sceneCreationFunc;

    // UI elements
    void createPauseButton();

    // Game state handling
    void handlePause();
    void handleResume();
};

#endif // BASE_SCENE_H

#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class MainMenu : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(MainMenu);

private:
    void startLoading();
    bool onTouchBegan(Touch* touch, Event* unused_event);
    bool onTouchMoved(Touch* touch, Event* unused_event);
    bool onTouchEnded(Touch* touch, Event* unused_event);

    cocos2d::ui::LoadingBar* loadingBar;
};

#endif // __MAINMENU_SCENE_H__

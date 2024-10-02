#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class MainMenu : public Scene {
public:
    static Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(MainMenu);

private:
    void startLoading(); // Method to start loading the next scene
};

#endif // __MAIN_MENU_SCENE_H__

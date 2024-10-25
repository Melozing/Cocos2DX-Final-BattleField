#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Game2/Cursor/Cursor.h"

USING_NS_CC;

class MainMenu : public Scene {
public:
    static Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(MainMenu);

private:
    void startLoading(std::string nameScene); // Method to start loading the next scene
    Cursor* _cursor;
};

#endif // __MAIN_MENU_SCENE_H__

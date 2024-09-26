#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class LoadingScene : public Scene
{
public:
    static Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(LoadingScene);

    void startLoading();

private:
    ui::LoadingBar* loadingBar;
};

#endif // __LOADING_SCENE_H__

#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class LoadingScene : public Scene {
public:
    static Scene* createScene(const std::string& sceneName);

    virtual bool init() override;

    CREATE_FUNC(LoadingScene);

    void startLoading();
    void setNextSceneName(const std::string& sceneName);

private:
    ui::LoadingBar* loadingBar;
    Sprite* border;
    std::string nextSceneName; // Store the name of the next scene
};

#endif // __LOADING_SCENE_H__

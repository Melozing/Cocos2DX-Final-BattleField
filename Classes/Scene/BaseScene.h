#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "cocos2d.h"
#include "Button/PauseButton.h"

class BaseScene : public cocos2d::Scene {
public:
    virtual bool init() override;
    void setSceneCreationFunc(const std::function<cocos2d::Scene* ()>& func);

    CREATE_FUNC(BaseScene);

protected:
    PauseButton* _pauseButton;
private:
    std::function<cocos2d::Scene* ()> _sceneCreationFunc;
};

#endif // BASE_SCENE_H

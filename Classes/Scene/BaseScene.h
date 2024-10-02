#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "cocos2d.h"
#include "Button/PauseButton.h"

class BaseScene : public cocos2d::Scene {
public:
    virtual bool init() override;
    CREATE_FUNC(BaseScene);

protected:
    PauseButton* _pauseButton;
};

#endif // BASE_SCENE_H

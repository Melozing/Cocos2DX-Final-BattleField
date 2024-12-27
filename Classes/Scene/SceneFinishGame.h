#ifndef __SCENE_FINISH_GAME_H__
#define __SCENE_FINISH_GAME_H__

#include "cocos2d.h"
#include "json/document.h"

class SceneFinishGame : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(SceneFinishGame);

private:
    void loadCredits(const std::string& jsonFilePath);
    void createScrollingCredits(const rapidjson::Document& document);
};

#endif // __SCENE_FINISH_GAME_H__

#ifndef __SCENE_CONTROLLER_H__
#define __SCENE_CONTROLLER_H__

#include "cocos2d.h"
#include <unordered_map>
#include <functional>

USING_NS_CC;

class SceneController {
public:
    static SceneController* getInstance();

    Scene* getScene(const std::string& sceneName);

    // Register a scene with its name and creation function
    void registerScene(const std::string& sceneName, std::function<Scene* ()> createFunc);

    // Create a scene by its name
    Scene* createScene(const std::string& sceneName);

private:
    static SceneController* instance; // Static instance of the singleton
    std::unordered_map<std::string, std::function<Scene* ()>> sceneMap; // Map for scene registration
};

#endif // __SCENE_CONTROLLER_H__

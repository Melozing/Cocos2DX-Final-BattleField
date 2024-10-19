#include "SceneController.h"
#include "Game1/Game1Scene.h"
#include "Game2/Game2Scene.h"
#include "Game3/Game3Scene.h"

SceneController* SceneController::instance = nullptr;

SceneController* SceneController::getInstance() {
    if (instance == nullptr) {
        instance = new SceneController();
        instance->init();
    }
    return instance;
}

bool SceneController::init() {
    registerScenes();
    return true;
}

Scene* SceneController::getScene(const std::string& sceneName) {
    return createScene(sceneName);
}

Scene* SceneController::createScene(const std::string& sceneName) {

    static std::unordered_map<std::string, Scene*> sceneCache;
    if (sceneCache.find(sceneName) != sceneCache.end()) {
        return sceneCache[sceneName];
    }

    auto it = sceneMap.find(sceneName);
    if (it != sceneMap.end()) {
        Scene* scene = it->second();
        sceneCache[sceneName] = scene;
        return scene;
    }
    return nullptr;
}


void SceneController::registerScene(const std::string& sceneName, std::function<Scene* ()> createFunc) {
    sceneMap[sceneName] = createFunc;
}

void SceneController::registerScenes() {
    registerScene("Game1Scene", []() {
        return Game1Scene::createScene();
        });

    registerScene("Game2Scene", []() {
        return Game2Scene::createScene();
        });
    registerScene("Game3Scene", []() {
        return Game3Scene::createScene();
        });
}

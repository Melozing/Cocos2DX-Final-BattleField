#include "SceneController.h"
#include "Constants/Constants.h"
#include "Scene/MainMenuScene.h"
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
        sceneCache[sceneName]->release();
        sceneCache.erase(sceneName);
    }

    auto it = sceneMap.find(sceneName);
    if (it != sceneMap.end()) {
        Scene* scene = it->second();
        sceneCache[sceneName] = scene;
        scene->retain();
        return scene;
    }
    return nullptr;
}


void SceneController::registerScene(const std::string& sceneName, std::function<Scene* ()> createFunc) {
    sceneMap[sceneName] = createFunc;
}

void SceneController::registerScenes() {
    registerScene(Constants::MAINMENU_SCENE_NAME, []() {
        return MainMenu::createScene();
        });

    registerScene(Constants::GAME1_SCENE_NAME, []() {
        return Game1Scene::createScene();
        });

    registerScene(Constants::GAME2_SCENE_NAME, []() {
        return Game2Scene::createScene();
        });
    registerScene(Constants::GAME3_SCENE_NAME, []() {
        return Game3Scene::createScene();
        });
}

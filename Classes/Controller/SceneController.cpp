#include "SceneController.h"
#include "Game1/Game1Scene.h"
#include "Game2/Game2Scene.h"
#include "Game3/Game3Scene.h"

SceneController* SceneController::instance = nullptr;

SceneController* SceneController::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SceneController();
        instance->init();
    }
    return instance;
}

bool SceneController::init() {
    return true;
}

Scene* SceneController::getScene(const std::string& sceneName) {
    return createScene(sceneName); // Use your createScene method to get the scene.
}

Scene* SceneController::createScene(const std::string& sceneName) {
    auto it = sceneMap.find(sceneName);
    if (it != sceneMap.end()) {
        return it->second(); // Call the function to create the scene
    }
    return nullptr; // Return nullptr if the scene is not found
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

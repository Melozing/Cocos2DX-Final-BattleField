#include "LoadingScene.h"
#include "Game1/Game1Scene.h"
#include "Game2/Game2Scene.h"
#include "Controller/SceneController.h"
#include "Controller/SpriteController.h"

Scene* LoadingScene::createScene(const std::string& sceneName) {
    auto scene = LoadingScene::create();
    scene->setNextSceneName(sceneName);
    return scene;
}

bool LoadingScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create and position the loading bar
    loadingBar = ui::LoadingBar::create("assets_game/UXUI/Loading/Loading_Bar_A.png");
    loadingBar->setPercent(0);
    loadingBar->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + loadingBar->getContentSize().height + SpriteController::calculateScreenRatio(0.05f)));

    // Create and position the border
    border = Sprite::create("assets_game/UXUI/Loading/Loading_Bar_Border.png");
    auto loadingPos = loadingBar->getPosition();
    float loadingBarHeight = loadingBar->getContentSize().height * loadingBar->getScaleY() + loadingBar->getScaleY();
    loadingPos.y += loadingBarHeight;
    border->setPosition(loadingPos);
    this->addChild(border, -1); // Place border behind the loading bar

    // Add loading bar to the scene
    this->addChild(loadingBar);

    // Start loading after a brief delay
    loadingBar->setVisible(false); // Initially hide the loading bar
    this->scheduleOnce([this](float) {
        loadingBar->setVisible(true);
        startLoading();
        }, 0.5f, "start_loading_key"); // Delay before starting the loading

    return true;
}

void LoadingScene::setNextSceneName(const std::string& sceneName) {
    nextSceneName = sceneName;
}

void LoadingScene::startLoading() {
    this->schedule([this](float dt) {
        float percent = loadingBar->getPercent();
        percent += 0.5f; // Increase loading percentage
        loadingBar->setPercent(percent);

        if (percent >= 100) {
            loadingBar->setPercent(100);
            this->unschedule("loading_bar_update_key");

            // Use SceneController to get the scene
            auto nextScene = SceneController::getInstance()->getScene(nextSceneName);
            if (nextScene) {
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, nextScene));
            }
            else {
                CCLOG("Scene not found: %s", nextSceneName.c_str());
            }
        }
        }, 0.2f / 60.0f, "loading_bar_update_key");
}

#include "LoadingScene.h"
#include "Controller/SceneController.h"
#include "Controller/SpriteController.h"
#include "Manager/BackgroundManager.h"

Scene* LoadingScene::createScene(const std::string& nextSceneName) {
    auto scene = Scene::create();
    auto layer = LoadingScene::create();
    layer->setNextSceneName(nextSceneName);
    scene->addChild(layer);
    return scene;
}

bool LoadingScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Set background
    BackgroundManager::getInstance()->setBackground(this, "assets_game/UXUI/Background/background_ui.jpg");

    // Create and position the loading bar
    auto _spriteLoading = Sprite::create("assets_game/UXUI/Loading/Loading_Bar_A.png");
    auto _spriteLoadingBorder = Sprite::create("assets_game/UXUI/Loading/Loading_Bar_Border.png");
    loadingBar = ui::LoadingBar::create("assets_game/UXUI/Loading/Loading_Bar_A.png");
    loadingBar->setPercent(0);
    loadingBar->setScale(SpriteController::updateSpriteScale(_spriteLoading, 0.57f));
    loadingBar->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + loadingBar->getContentSize().height + SpriteController::calculateScreenRatio(0.05f)));

    // Create and position the border
    border = Sprite::create("assets_game/UXUI/Loading/Loading_Bar_Border.png");
    border->setScale(SpriteController::updateSpriteScale(_spriteLoadingBorder, 0.6f));
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

    static bool transitionStarted = false;
    if (!transitionStarted) {
        transitionStarted = true;
        this->scheduleOnce([this](float) {
            auto nextScene = SceneController::getInstance()->getScene(nextSceneName);
            if (nextScene) {
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, nextScene));
            }
            }, 2.0f, "loading_key");
    }
    return true;
}


void LoadingScene::setNextSceneName(const std::string& sceneName) {
    nextSceneName = sceneName;
}

void LoadingScene::startLoading() {
    auto currentScene = Director::getInstance()->getRunningScene();
    if (currentScene) {
        CCLOG("Current Scene: %s", typeid(*currentScene).name());
    }
    else {
        CCLOG("No current scene running.");
    }

    CCLOG("Next Scene Name: %s", nextSceneName.c_str());

    this->schedule([this](float dt) {
        float percent = loadingBar->getPercent();
        percent += 0.5f; // Increase loading percentage
        loadingBar->setPercent(percent);


        if (percent >= 100) {
            loadingBar->setPercent(100);
            this->unschedule("loading_bar_update_key");

            auto nextScene = SceneController::getInstance()->getScene(nextSceneName);
            if (nextScene) {
                CCLOG("Next Scene: %s", typeid(*nextScene).name());
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, nextScene));
            }
            else {
                CCLOG("Scene not found: %s", nextSceneName.c_str());
            }
        }
        }, 0.5f / 60.0f, "loading_bar_update_key");
}


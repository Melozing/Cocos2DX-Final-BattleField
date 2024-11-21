#include "LoadingScene.h"
#include "LoadingBar/CustomLoadingBar.h"
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

    initCursor();
    initBackground();
    initLoadingBar();

    // Start loading after a brief delay
    loadingBar->setVisible(false); // Initially hide the loading bar
    this->scheduleOnce([this](float) {
        loadingBar->setVisible(true);
        startLoading();
        }, 0.5f, "start_loading_key"); // Delay before starting the loading
    return true;
}

void LoadingScene::initCursor() {
    // Create and add the custom cursor
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/UXUI/Main_Menu/pointer.png");
    _cursor->setAnchorPoint(Vec2(0.5, 0.5));
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f));
    _cursor->setVisible(true);
    _cursor->setInitialPosition();
    if (_cursor) {
        this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR); // Add cursor to the scene with z-order 1
    }
}

void LoadingScene::initBackground() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    BackgroundManager::getInstance()->setBackground(this, "assets_game/UXUI/Background/background_ui.jpg");
}

void LoadingScene::initLoadingBar() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    customLoadingBar = CustomLoadingBar::create("assets_game/UXUI/Loading/Loading_Bar_A.png", "assets_game/UXUI/Loading/Loading_Bar_Border.png", 0.57f);
    customLoadingBar->setLoadingBarPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + customLoadingBar->getLoadingBar()->getContentSize().height + SpriteController::calculateScreenRatio(0.05f)));

    auto loadingPos = customLoadingBar->getLoadingBar()->getPosition();
    float loadingBarHeight = customLoadingBar->getLoadingBar()->getContentSize().height * customLoadingBar->getLoadingBar()->getScaleY() + SpriteController::calculateScreenRatio(0.001f);
    loadingPos.y += loadingBarHeight;
    customLoadingBar->setBorderPosition(loadingPos);

    customLoadingBar->setLoadingBarScale(SpriteController::updateSpriteScale(customLoadingBar->getLoadingBar(), 0.57f));
    customLoadingBar->setBorderScale(SpriteController::updateSpriteScale(customLoadingBar->getBorder(), 0.6f));

    this->addChild(customLoadingBar);

    loadingBar = customLoadingBar->getLoadingBar();
}

void LoadingScene::setNextSceneName(const std::string& sceneName) {
    nextSceneName = sceneName;
}

void LoadingScene::startLoading() {
    if (!loadingBar) {
        CCLOG("Loading bar is not initialized.");
        return;
    }

    auto currentScene = Director::getInstance()->getRunningScene();

    this->schedule([this](float dt) {
        float percent = loadingBar->getPercent();
        percent += 0.5f; // Increase loading percentage
        loadingBar->setPercent(percent);

        if (percent >= 100) {
            loadingBar->setPercent(100);
            this->unschedule("loading_bar_update_key");

            auto nextScene = SceneController::getInstance()->getScene(nextSceneName);
            if (nextScene) {
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, nextScene));
            }
            else {
                CCLOG("Scene not found: %s", nextSceneName.c_str());
            }
        }
        }, 0.5f / 60.0f, "loading_bar_update_key");
}


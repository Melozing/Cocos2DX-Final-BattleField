#include "LoadingScene.h"
#include "Game1/Game1Scene.h"
#include "Game2/Game2Scene.h"

USING_NS_CC;

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

Scene* createSceneByName(const std::string& sceneName) {
    if (sceneName == "Game1") {
        return Game1Scene::createScene();
    }
    else if (sceneName == "Game2") {
        return Game2Scene::createScene();
    }
    // Add more scenes as needed
    return nullptr; // Return nullptr if no matching scene is found
}

void LoadingScene::startLoading() {
    this->schedule([this](float dt) {
        float percent = loadingBar->getPercent();
        percent += 0.5f; // Increase by 0.5 for smoothness
        loadingBar->setPercent(percent);

        if (percent >= 100) {
            loadingBar->setPercent(100);
            this->unschedule("loading_bar_update_key");

            this->runAction(Sequence::create(
                CallFunc::create([this]() {
                    Scene* nextScene = createSceneByName(nextSceneName);
                    if (nextScene) {
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5, nextScene));
                    }
                    else {
                        CCLOG("Scene not found: %s", nextSceneName.c_str());
                    }
                    }),
                nullptr
            ));
        }
        }, 0.2f / 60.0f, "loading_bar_update_key");
}

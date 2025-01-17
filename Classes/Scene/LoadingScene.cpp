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

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/UXUI/Loading/LoadingIcon.plist");
    initBackground();
    initLoadingBar();

    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_WINDOWS ||
        platform == cocos2d::Application::Platform::OS_MAC) {
        initCursor();
    }
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
    BackgroundManager::getInstance()->setBackgroundWithOverlay(this, "assets_game/UXUI/Background/bg_loading.jpg");
}

void LoadingScene::initLoadingBar() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    customLoadingBar = CustomLoadingBar::create("assets_game/UXUI/Loading/Loading_Bar_A.png", "assets_game/UXUI/Loading/Loading_Bar_Border.png", 0.57f);
    customLoadingBar->setLoadingBarPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + customLoadingBar->getLoadingBar()->getContentSize().height + SpriteController::calculateScreenRatio(0.05f)));

    auto loadingPos = customLoadingBar->getLoadingBar()->getPosition();
    customLoadingBar->setBorderPosition(loadingPos);

    customLoadingBar->setLoadingBarScale(SpriteController::updateSpriteScale(customLoadingBar->getLoadingBar(), 0.57f));
    customLoadingBar->setBorderScale(SpriteController::updateSpriteScale(customLoadingBar->getBorder(), 0.6f));

    this->addChild(customLoadingBar);

    loadingBar = customLoadingBar->getLoadingBar();

    // Add nail sprites to the left and right corners of the loading bar
    auto nailLeft = Sprite::create("assets_game/UXUI/Loading/nail_left.png");
    auto nailRight = Sprite::create("assets_game/UXUI/Loading/nail_right.png");

	auto paddingWidth = SpriteController::calculateScreenRatio(0.015f);
    
    // Set positions for the nail sprites
    nailLeft->setPosition(Vec2(loadingPos.x - SpriteController::GetContentSize(customLoadingBar->getBorder()).width / 2 + paddingWidth, loadingPos.y));
    nailRight->setPosition(Vec2(loadingPos.x + SpriteController::GetContentSize(customLoadingBar->getBorder()).width / 2 - paddingWidth, loadingPos.y));

    // Set scales for the nail sprites
    nailLeft->setScale(SpriteController::updateSpriteScale(nailLeft, 0.03f));
    nailRight->setScale(SpriteController::updateSpriteScale(nailRight, 0.03f));

    // Add nail sprites to the scene
    this->addChild(nailLeft, Constants::ORDER_LAYER_UI + 9999);
    this->addChild(nailRight, Constants::ORDER_LAYER_UI + 9999);

    // Initialize the loading icon
    initLoadingIcon();
}

void LoadingScene::initLoadingIcon() {
    loadingIconBatchNode = SpriteBatchNode::create("assets_game/UXUI/Loading/LoadingIcon.png");
    if (loadingIconBatchNode->getParent() == nullptr) {
        this->addChild(loadingIconBatchNode);
    }

    loadingIcon = Sprite::createWithSpriteFrameName("LoadingIcon1.png");
    loadingIcon->setScale(SpriteController::updateSpriteScale(loadingIcon, 0.1f));

    auto loadingPos = customLoadingBar->getLoadingBar()->getPosition();
    loadingIcon->setPosition(Vec2(loadingPos.x, loadingPos.y + SpriteController::calculateScreenRatio(0.035f)));

    loadingIconBatchNode->addChild(loadingIcon);

    auto animate = Animate::create(SpriteController::createAnimation("LoadingIcon", 39, 0.033));
    loadingIcon->runAction(RepeatForever::create(animate));
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


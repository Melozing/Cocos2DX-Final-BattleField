#include "BaseScene.h"
#include "Controller/GameController.h"
#include "Controller/SceneController.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"

USING_NS_CC;

bool BaseScene::init() {
    if (!Scene::init()) {
        return false;
    }
    GameController::getInstance()->setTutorialLayerActive(false);

    // Add keyboard event listener for pause and resume functionality
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_P) {
            handlePause();
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
            handleResume();
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Register for notifications
    registerNotification();

    // Create the pause button
    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        createPauseButton();
    }


    return true;
}

void BaseScene::createPauseButton() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float padding = SpriteController::calculateScreenRatio(0.1f);

    // Create the pause button
    auto pauseButton = MenuItemImage::create(
        "assets_game/UXUI/Panel/Play_BTN.png", // Normal image
        "assets_game/UXUI/Panel/Play_BTN_Active.png", // Selected image
        [this](Ref* sender) {
            handlePause();
        }
    );

    pauseButton->setScale(SpriteController::updateSpriteScale(pauseButton, 0.1f));

    // Set the position of the pause button
    pauseButton->setPosition(Vec2(origin.x + visibleSize.width - padding / 2,
        origin.y + visibleSize.height - padding / 2));

    // Set the opacity of the pause button to 50%
    pauseButton->setOpacity(128);

    // Create the menu and add the pause button to it
    auto menu = Menu::create(pauseButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

void BaseScene::handlePause() {
    if (GameController::getInstance()->isPaused() || GameController::getInstance()->isTutorialLayerActive()) return;
    if (!GameController::getInstance()->isGameOver()) {
        if (_cursor) {
            _cursor->setVisible(false);
        }
        auto exitAction = []() {
            Director::getInstance()->end();
            };
        std::string soundtrackPath = Constants::currentSoundTrackPath;
        SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
        GameController::getInstance()->pauseGame(exitAction, _sceneCreationFunc, soundtrackPath);
    }
}

void BaseScene::handleResume() {
    if (GameController::getInstance()->isPaused() || GameController::getInstance()->isTutorialLayerActive()) {
        if (_cursor) {
            _cursor->setVisible(true);
        }
        SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
        GameController::getInstance()->resumeGame();
    }
}

void BaseScene::setSceneCreationFunc(const std::function<cocos2d::Scene* ()>& func) {
    _sceneCreationFunc = func;
}

void BaseScene::registerNotification() {
    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(BaseScene::hideCursor),
        "HideCursorNotification",
        nullptr
    );

    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(BaseScene::showCursor),
        "ShowCursorNotification",
        nullptr
    );
}

void BaseScene::hideCursor(Ref* sender) {
    if (_cursor) {
        _cursor->setVisible(false);
    }
}

void BaseScene::showCursor(Ref* sender) {
    if (_cursor) {
        _cursor->setVisible(true);
    }
}

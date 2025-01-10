/*#include "UIGame/IntroScene.h"
#include "Scene/MainMenuScene.h"
#include "ui/UIButton.h"
#include "ui/UIVideoPlayer.h"

USING_NS_CC;

Scene* IntroScene::createScene() {
    return IntroScene::create();
}

bool IntroScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Create a black background layer
    auto blackLayer = LayerColor::create(Color4B::BLACK);
    this->addChild(blackLayer, -1);

    // Create a VideoPlayer
    auto videoPlayer = cocos2d::ui::VideoPlayer::create();
    videoPlayer->setFileName("intro.mp4");
    videoPlayer->setContentSize(visibleSize);
    videoPlayer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    videoPlayer->setKeepAspectRatioEnabled(true);
    videoPlayer->setFullScreenEnabled(false);
    videoPlayer->addEventListener(CC_CALLBACK_2(IntroScene::onVideoEvent, this));
    this->addChild(videoPlayer);

    // Play the video after 2 seconds of black screen
    this->scheduleOnce([videoPlayer](float dt) {
        videoPlayer->play();
        }, 2.0f, "play_video_key");

    // Schedule to allow skipping after 10 seconds
    _canSkip = false;
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(IntroScene::onSkipIntro), 10.0f);

    // Add touch listener to skip the intro
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (_canSkip) {
            startMainMenuScene();
        }
        return true;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void IntroScene::onVideoEvent(Ref* sender, cocos2d::ui::VideoPlayer::EventType eventType) {
    if (eventType == cocos2d::ui::VideoPlayer::EventType::COMPLETED) {
        // Add 1 second of black screen after video ends
        this->scheduleOnce([this](float dt) {
            startMainMenuScene();
            }, 1.0f, "end_video_key");
    }
}

void IntroScene::onSkipIntro(float dt) {
    _canSkip = true;
}

void IntroScene::startMainMenuScene() {
    auto scene = MainMenu::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

*/
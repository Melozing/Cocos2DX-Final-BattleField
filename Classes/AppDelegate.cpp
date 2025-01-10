#include "AppDelegate.h"
#include "Scene/MainMenuScene.h"
#include "Constants/Constants.h"
#include "Controller/GameController.h"
#include "Controller/SoundController.h"
// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

// Design resolution size with 16:9 aspect ratio (Full HD as default)
static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // Initialize Director and GLView
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // Create a window with 16:9 aspect ratio (Full HD)
        glview = GLViewImpl::createWithRect("FinalBattleField", Rect(100, 100, 1280, 720));
#else
        glview = GLViewImpl::create("FinalBattleField");
#endif
        director->setOpenGLView(glview);
    }

    // Set the design resolution size
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    Size frameSize = glview->getFrameSize();

    // Adjust content scale factor based on the frame size
    if (frameSize.height > mediumResolutionSize.height) {
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    }
    else if (frameSize.height > smallResolutionSize.height) {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    }
    else {
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    }

    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    // Register packages
    register_all_packages();

    // Initialize GameController
    GameController::getInstance();
    GameController::getInstance()->init();

    // Set default volume values if not set
    auto userDefault = UserDefault::getInstance();
    if (!userDefault->getBoolForKey("isVolumeInitialized", false)) {
        userDefault->setFloatForKey(Constants::UD_effectsVolume.c_str(), 0.5f);
        userDefault->setFloatForKey(Constants::UD_musicVolume.c_str(), 0.5f);
        userDefault->setBoolForKey("isVolumeInitialized", true);
        userDefault->flush();
    }

    SoundController::getInstance()->setEffectsVolume(UserDefault::getInstance()->getFloatForKey(Constants::UD_effectsVolume.c_str()));
    SoundController::getInstance()->setMusicVolume(UserDefault::getInstance()->getFloatForKey(Constants::UD_musicVolume.c_str()));

    auto scene = MainMenu::createScene();
    //auto scene = IntroScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}

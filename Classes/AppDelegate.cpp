#include "AppDelegate.h"
#include "Scene/MainMenuScene.h"
#include "Controller/GameController.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size smallResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        glview = GLViewImpl::createWithRect("FinalBattleField", Rect(0, 0, screenWidth, screenHeight));

        // Windows specific code to make the window borderless
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        HWND hwnd = glview->getWin32Window();
        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        style &= ~(WS_CAPTION | WS_THICKFRAME);
        SetWindowLong(hwnd, GWL_STYLE, style);

        LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
        SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);

        // Adjust window size and position to match the screen size
        SetWindowPos(hwnd, NULL, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
#endif

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        // Mac specific code to make the window borderless
        // Replace with actual code to make the window borderless on Mac
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // Linux specific code to make the window borderless
        // Replace with actual code to make the window borderless on Linux
#else
        glview = GLViewImpl::create("FinalBattleField");
#endif
        director->setOpenGLView(glview);
    }

    glview->setCursorVisible(true);
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 120);

    // Set the design resolution size to match the screen size
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    glview->setDesignResolutionSize(screenWidth, screenHeight, ResolutionPolicy::NO_BORDER);

    register_all_packages();

    GameController::getInstance();
    GameController::getInstance()->init();

    auto scene = MainMenu::createScene();
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

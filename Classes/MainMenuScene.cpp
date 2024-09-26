#include "MainMenuScene.h"
#include "Game1/Game1Scene.h"
#include "Scene/LoadingScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

bool MainMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto button = ui::Button::create("assets_game/UXUI/Main_Menu/Start_BTN.png");
    button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            startLoading();
        }
        });
    this->addChild(button);

    loadingBar = ui::LoadingBar::create("assets_game/UXUI/Loading/Loading_Bar_A.png");
    loadingBar->setPercent(0);
    loadingBar->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));
    this->addChild(loadingBar);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(MainMenu::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(MainMenu::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(MainMenu::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void MainMenu::startLoading()
{
    this->schedule([this](float dt) {
        float percent = loadingBar->getPercent();
        percent += 1; // Increase loading bar smoothly

        loadingBar->setPercent(percent);

        if (percent >= 100) {
            loadingBar->setPercent(100);
            this->unschedule("loading_bar_update_key");

            auto loadingScene = LoadingScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, loadingScene));
        }
        }, 0.1f, "loading_bar_update_key"); // Update every 0.1 seconds
}

bool MainMenu::onTouchBegan(Touch* touch, Event* unused_event) {
    return true;
}

bool MainMenu::onTouchMoved(Touch* touch, Event* unused_event) {
    return true;
}

bool MainMenu::onTouchEnded(Touch* touch, Event* unused_event) {
    return true;
}

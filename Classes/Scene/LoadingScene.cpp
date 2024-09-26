#include "LoadingScene.h"
#include "Game1/Game1Scene.h"
#include "Game2/Game2Scene.h"

USING_NS_CC;

Scene* LoadingScene::createScene()
{
    return LoadingScene::create();
}

bool LoadingScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    loadingBar = ui::LoadingBar::create("assets_game/UXUI/Loading/Loading_Bar_A.png");
    loadingBar->setPercent(0);
    loadingBar->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(loadingBar);

    startLoading();

    return true;
}

void LoadingScene::startLoading()
{
    // Smoothly increase the loading bar's percentage
    this->schedule([this](float dt) {
        float percent = loadingBar->getPercent();
        percent += 0.5f; // Increase by 0.5 for smoothness
        loadingBar->setPercent(percent);

        // If the loading reaches 100%, stop the schedule and transition to the next scene
        if (percent >= 100) {
            loadingBar->setPercent(100); // Ensure it stays at 100%
            this->unschedule("loading_bar_update_key");

            // Delay before switching scenes
            this->runAction(Sequence::create(
                CallFunc::create([]() {
                    auto gameScene = Game1Scene::createScene(); // Change to your desired scene
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameScene));
                    }),
                nullptr
            ));
        }
        }, 0.2f / 60.0f, "loading_bar_update_key"); // Update every frame
}

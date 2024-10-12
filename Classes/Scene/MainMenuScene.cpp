#include "Scene/MainMenuScene.h"
#include "Controller/SceneController.h"
#include "Scene/LoadingScene.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

Scene* MainMenu::createScene() {
    return MainMenu::create();
}

bool MainMenu::init() {
    if (!Scene::init()) {
        return false; // Ensure initialization is successful
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto button = ui::Button::create("assets_game/UXUI/Main_Menu/Start_BTN.png");
	auto sprite = Sprite::create("assets_game/UXUI/Main_Menu/Start_BTN.png");
    button->setScale(SpriteController::updateSpriteScale(sprite, 0.08f));
    button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            startLoading(); // Start loading when button is pressed
        }
        });
    this->addChild(button); // Add button to the scene

    return true; // Initialization succeeded
}

void MainMenu::startLoading() {
    auto loadingScene = LoadingScene::createScene("Game1Scene");
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, loadingScene)); // Replace with LoadingScene
}

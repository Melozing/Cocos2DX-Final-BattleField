#include "Panel/VictoryPanel.h"
#include "ui/CocosGUI.h"
#include "Controller/GameController.h"
#include "Scene/LoadingScene.h"

USING_NS_CC;

VictoryPanel* VictoryPanel::createPanel(const std::function<void()>& replayAction, const std::function<void()>& exitAction) {
    return VictoryPanel::create(replayAction, exitAction);
}

VictoryPanel* VictoryPanel::create(const std::function<void()>& replayAction, const std::function<void()>& exitAction) {
    VictoryPanel* ret = new (std::nothrow) VictoryPanel();
    if (ret && ret->init(replayAction, exitAction)) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

bool VictoryPanel::init(const std::function<void()>& replayAction, const std::function<void()>& exitAction) {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create background
    auto background = ui::Layout::create();
    background->setContentSize(visibleSize);
    background->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    background->setBackGroundColor(Color3B(0, 0, 0));
    background->setBackGroundColorOpacity(150);
    this->addChild(background);

    // Create a label for the victory message
    auto label = Label::createWithTTF("You Win!", "fonts/Marker Felt.ttf", 32);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 50));
    this->addChild(label);

    // Create a replay button
    auto replayButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    replayButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 50));
    replayButton->addClickEventListener([replayAction](Ref* sender) {
        replayAction();
        });
    this->addChild(replayButton);

    // Create an exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Close_BTN.png");
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50, origin.y + visibleSize.height / 2 - 50));
    exitButton->addClickEventListener([exitAction](Ref* sender) {
        exitAction();
        });
    this->addChild(exitButton);

    // Create a next level button
    auto nextLevelButton = ui::Button::create("assets_game/UXUI/Panel/Forward_BTN.png", "assets_game/UXUI/Panel/Forward_BTN_Active.png");
    nextLevelButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
    nextLevelButton->addClickEventListener([](Ref* sender) {
        auto loadingScene = LoadingScene::createScene("Game2Scene");
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, loadingScene));
        });
    this->addChild(nextLevelButton);

    return true;
}

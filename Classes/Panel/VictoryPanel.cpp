#include "Panel/VictoryPanel.h"
#include "Controller/GameController.h"
#include "Constants/Constants.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

VictoryPanel* VictoryPanel::createPanel(const std::function<void()>& replayAction, const std::function<void()>& exitAction) {
    VictoryPanel* panel = new (std::nothrow) VictoryPanel();
    if (panel && panel->init(replayAction, exitAction)) {
        panel->autorelease();
        return panel;
    }
    CC_SAFE_DELETE(panel);
    return nullptr;
}

bool VictoryPanel::init(const std::function<void()>& replayAction, const std::function<void()>& exitAction) {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create a semi-transparent background
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(background);

    // Create a replay button
    auto replayButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    replayButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 50));
    replayButton->addClickEventListener([replayAction](Ref* sender) {
        replayAction();
        });
    this->addChild(replayButton);

    // Create an exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Exit_BTN.png", "assets_game/UXUI/Panel/Exit_BTN_Active.png");
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50, origin.y + visibleSize.height / 2 - 50));
    exitButton->addClickEventListener([exitAction](Ref* sender) {
        exitAction();
        });
    this->addChild(exitButton);

    return true;
}

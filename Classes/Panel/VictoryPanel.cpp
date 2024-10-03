#include "VictoryPanel.h"
#include "ui/CocosGUI.h" // Include Cocos GUI for ui::Layout and ui::Button

USING_NS_CC;

VictoryPanel* VictoryPanel::createPanel(const std::function<void()>& replayAction, const std::function<void()>& exitAction) {
    VictoryPanel* panel = VictoryPanel::create();
    if (panel) {
        panel->_replayAction = replayAction;
        panel->_exitAction = exitAction;
    }
    return panel;
}

bool VictoryPanel::init() {
    if (!Layer::init()) return false;

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
    auto replayButton = ui::Button::create("assets_game/ui/replay_button.png");
    replayButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 50));
    replayButton->addClickEventListener([this](Ref*) {
        if (_replayAction) _replayAction();
        });
    this->addChild(replayButton);

    // Create an exit button
    auto exitButton = ui::Button::create("assets_game/ui/exit_button.png");
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50, origin.y + visibleSize.height / 2 - 50));
    exitButton->addClickEventListener([this](Ref*) {
        if (_exitAction) _exitAction();
        });
    this->addChild(exitButton);

    return true;
}

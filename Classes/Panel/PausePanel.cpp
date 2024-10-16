#include "PausePanel.h"
#include "Button/ReplayButton.h"
#include "Button/ExitButton.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

PausePanel* PausePanel::createPanel(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction) {
    PausePanel* panel = new (std::nothrow) PausePanel();
    if (panel && panel->init(resumeCallback, retryAction, exitAction)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool PausePanel::init(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction) {
    if (!Layout::init()) {
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
    background->setName("PauseBackground"); // Set name for the background
    this->addChild(background);

    // Create resume button
    auto resumeButton = ui::Button::create("assets_game/UXUI/Buttons/Pause_BTN.png", "assets_game/UXUI/Buttons/Pause_BTN_Active.png");
    resumeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
    resumeButton->addClickEventListener([resumeCallback](Ref* sender) {
        resumeCallback();
        });
    this->addChild(resumeButton);

    // Create retry button
    auto retryButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    retryButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    retryButton->addClickEventListener([retryAction](Ref* sender) {
        retryAction();
        });
    this->addChild(retryButton);

    // Create exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Close_BTN.png");
    exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
    exitButton->addClickEventListener([exitAction](Ref* sender) {
        exitAction();
        });
    this->addChild(exitButton);

    this->setName("PausePanel"); // Set name for the panel

    return true;
}

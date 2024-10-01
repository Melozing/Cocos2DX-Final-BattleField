// GameOverPanel.cpp
#include "GameOverPanel.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

GameOverPanel* GameOverPanel::createPanel(const std::function<void()>& retryCallback, const std::function<void()>& exitCallback) {
    GameOverPanel* panel = new (std::nothrow) GameOverPanel();
    if (panel && panel->init(retryCallback, exitCallback)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool GameOverPanel::init(const std::function<void()>& retryCallback, const std::function<void()>& exitCallback) {
    if (!Layer::init()) {
        return false;
    }

    // Create a semi-transparent background
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(background);

    // Create Retry button
    auto retryButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    retryButton->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2) + Vec2(0, 50));
    retryButton->addClickEventListener([retryCallback](Ref* sender) {
        retryCallback();
        });
    this->addChild(retryButton);

    // Create Exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Menu_BTN.png", "assets_game/UXUI/Panel/Menu_BTN_Active.png");
    exitButton->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2) - Vec2(0, 50));
    exitButton->addClickEventListener([exitCallback](Ref* sender) {
        exitCallback();
        });
    this->addChild(exitButton);

    return true;
}

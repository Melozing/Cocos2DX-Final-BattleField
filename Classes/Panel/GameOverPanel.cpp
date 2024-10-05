#include "Panel/GameOverPanel.h"
#include "ui/CocosGUI.h"
#include "Controller/GameController.h"

USING_NS_CC;

GameOverPanel* GameOverPanel::createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction) {
    return GameOverPanel::create(retryAction, exitAction);
}

GameOverPanel* GameOverPanel::create(const std::function<void()>& retryAction, const std::function<void()>& exitAction) {
    GameOverPanel* ret = new (std::nothrow) GameOverPanel();
    if (ret && ret->init(retryAction, exitAction)) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

bool GameOverPanel::init(const std::function<void()>& retryAction, const std::function<void()>& exitAction) {
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

    // Create retry button
    auto retryButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    retryButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 50));
    retryButton->addClickEventListener([retryAction](Ref* sender) {
        retryAction();
        });
    this->addChild(retryButton);

    // Create exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Close_BTN.png");
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));
    exitButton->addClickEventListener([exitAction](Ref* sender) {
        exitAction();
        });
    this->addChild(exitButton);

    return true;
}

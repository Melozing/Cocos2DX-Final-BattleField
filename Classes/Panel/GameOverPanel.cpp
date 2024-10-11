#include "Panel/GameOverPanel.h"
#include "Button/ReplayButton.h"
#include "Button/ExitButton.h"
#include "ui/CocosGUI.h"
#include "Controller/GameController.h"
#include "Constants/Constants.h"

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
    auto retryButton = ReplayButton::create(retryAction);
    retryButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 50));
    this->addChild(retryButton);

    // Create exit button
    auto exitButton = ExitButton::create(exitAction);
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));
    this->addChild(exitButton);

    return true;
}

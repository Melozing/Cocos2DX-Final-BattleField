#include "Panel/VictoryPanel.h"
#include "Button/ReplayButton.h"
#include "Button/ExitButton.h"
#include "ui/CocosGUI.h"
#include "Controller/GameController.h"
#include "Constants/Constants.h"

USING_NS_CC;

VictoryPanel* VictoryPanel::createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction) {
    return VictoryPanel::create(retryAction, exitAction);
}

VictoryPanel* VictoryPanel::create(const std::function<void()>& retryAction, const std::function<void()>& exitAction) {
    VictoryPanel* ret = new (std::nothrow) VictoryPanel();
    if (ret && ret->init(retryAction, exitAction)) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

bool VictoryPanel::init(const std::function<void()>& retryAction, const std::function<void()>& exitAction) {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create a semi-transparent background
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(background);

    // Create a replay button
    auto replayButton = ReplayButton::create(retryAction);
    replayButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 50));
    this->addChild(replayButton);

    // Create an exit button
    auto exitButton = ExitButton::create(exitAction);
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50, origin.y + visibleSize.height / 2 - 50));
    this->addChild(exitButton);

    return true;
}


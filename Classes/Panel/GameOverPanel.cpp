#include "GameOverPanel.h"
#include "cocos2d.h"

USING_NS_CC;

GameOverPanel* GameOverPanel::createPanel(const std::function<void()>& retryAction,
    const std::function<void()>& exitAction) {
    GameOverPanel* panel = new GameOverPanel();
    if (panel && panel->init(retryAction, exitAction)) {
        panel->autorelease(); // Manage memory automatically
        return panel;
    }
    else {
        delete panel; // Cleanup if init fails
        return nullptr;
    }
}

bool GameOverPanel::init(const std::function<void()>& retryAction,
    const std::function<void()>& exitAction) {
    if (!Layer::init()) {
        return false;
    }

    this->retryAction = retryAction;
    this->exitAction = exitAction;

    setupUI(); // Set up the user interface

    this->setOpacity(0);
    this->runAction(FadeIn::create(1.0f)); // Fade in over 1 second

    return true;
}

void GameOverPanel::setupUI() {
    // Create a semi-transparent background
    auto background = LayerColor::create(Color4B(0, 0, 0, 200));
    this->addChild(background);

    // Create a title label
    auto titleLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 48);
    titleLabel->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2,
        Director::getInstance()->getWinSize().height - 100));
    this->addChild(titleLabel);

    // Create a retry button
    auto retryButton = MenuItemLabel::create(
        Label::createWithTTF("Retry", "fonts/Marker Felt.ttf", 36),
        CC_CALLBACK_1(GameOverPanel::onRetryButtonClicked, this));
    retryButton->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2,
        Director::getInstance()->getWinSize().height / 2 + 50));

    // Create an exit button
    auto exitButton = MenuItemLabel::create(
        Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 36),
        CC_CALLBACK_1(GameOverPanel::onExitButtonClicked, this));
    exitButton->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2,
        Director::getInstance()->getWinSize().height / 2 - 50));

    // Create menu and add buttons
    auto menu = Menu::create(retryButton, exitButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void GameOverPanel::onRetryButtonClicked(Ref* sender) {
    if (retryAction) {
        retryAction(); // Call the retry action
    }
    fadeOutAndRemove(); // Fade out and then remove the panel
}

void GameOverPanel::onExitButtonClicked(Ref* sender) {
    if (exitAction) {
        exitAction(); // Call the exit action
    }
    fadeOutAndRemove(); // Fade out and then remove the panel
}

void GameOverPanel::fadeOutAndRemove() {
    // Fade out over 0.5 seconds and then remove the panel
    auto fadeOutAction = FadeOut::create(0.5f);
    auto removeAction = CallFunc::create([this]() {
        this->removeFromParent(); // Remove the panel after fade out
        });
    this->runAction(Sequence::create(fadeOutAction, removeAction, nullptr));
}

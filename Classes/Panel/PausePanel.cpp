#include "PausePanel.h"
#include "Button/ReplayButton.h"
#include "Button/ExitButton.h"
#include "Scene/LoadingScene.h"
#include "Constants/Constants.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

PausePanel* PausePanel::createPanel(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction) {
    PausePanel* panel = new (std::nothrow) PausePanel();
    if (panel && panel->init(resumeCallback, retryAction, exitAction, backAction)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool PausePanel::init() {
    if (!BasePanel::init()) {
        return false;
    }
    return true;
}

bool PausePanel::init(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction) {
    if (!BasePanel::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Close_BTN.png");
    exitButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    exitButton->setPosition(Vec2(boardSprite->getContentSize().width - exitButton->getContentSize().width / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_EXITBUTTON),
        boardSprite->getContentSize().height - exitButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_EXITBUTTON)));
    exitButton->addClickEventListener([resumeCallback](Ref* sender) {
        resumeCallback();
        });
    boardSprite->addChild(exitButton);

    // Create resume button
    auto resumeButton = ui::Button::create("assets_game/UXUI/Buttons/Pause_BTN.png", "assets_game/UXUI/Buttons/Pause_BTN_Active.png");
    resumeButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    resumeButton->setPosition(Vec2(boardSprite->getContentSize().width / 2 - resumeButton->getContentSize().width - SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_PANEL) / 2,
        resumeButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    resumeButton->addClickEventListener([resumeCallback](Ref* sender) {
        resumeCallback();
        });
    boardSprite->addChild(resumeButton);

    // Create retry button
    auto retryButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    retryButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    retryButton->setPosition(Vec2(boardSprite->getContentSize().width / 2 + retryButton->getContentSize().width + SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_PANEL) / 2,
        retryButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    retryButton->addClickEventListener([retryAction](Ref* sender) {
        retryAction();
        });
    boardSprite->addChild(retryButton);

    // Create back button
    auto backButton = ui::Button::create("assets_game/UXUI/Panel/Back_BTN.png", "assets_game/UXUI/Panel/Back_BTN_Active.png");
    backButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    backButton->setPosition(Vec2(boardSprite->getContentSize().width / 2,
        backButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    backButton->addClickEventListener([backAction](Ref* sender) {
        backAction();
        });
    boardSprite->addChild(backButton);

    // Create exit button
    cocos2d::ui::Button* exitButton2 = ui::Button::create("assets_game/UXUI/Panel/Exit_BTN.png", "assets_game/UXUI/Panel/Exit_BTN.png");
    exitButton2->setAnchorPoint(Vec2(0.5f, 0.5f));
    exitButton2->setPosition(Vec2(boardSprite->getContentSize().width / 2,
        boardSprite->getContentSize().height - exitButton2->getContentSize().height / 2 - SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_EXITBUTTON)));
    exitButton2->addClickEventListener([exitAction](Ref* sender) {
        exitAction();
        });
    boardSprite->addChild(exitButton2);

    return true;
}

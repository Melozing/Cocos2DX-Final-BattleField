#include "Panel/VictoryPanel.h"
#include "Button/ReplayButton.h"
#include "Button/ExitButton.h"
#include "ui/CocosGUI.h"
#include "Controller/GameController.h"
#include "Constants/Constants.h"

USING_NS_CC;

VictoryPanel* VictoryPanel::createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction, const std::function<void()>& nextSceneAction) {
    VictoryPanel* panel = new (std::nothrow) VictoryPanel();
    if (panel && panel->init(retryAction, exitAction, backAction, nextSceneAction)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool VictoryPanel::init(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction, const std::function<void()>& nextSceneAction) {
    if (!BasePanel::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    this->removeChild(boardSprite);

    // Create a new board sprite specific to GameOverPanel
    boardSprite = Sprite::create("assets_game/UXUI/Panel/Board_Victory.png");
    boardSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    boardSprite->setScale(SpriteController::updateSpriteScale(boardSprite, 0.525f));
    boardSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + SpriteController::calculateScreenRatio(0.03f)));
    this->addChild(boardSprite);

    // Create exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Close_BTN.png");
    exitButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    exitButton->setPosition(Vec2(boardSprite->getContentSize().width - exitButton->getContentSize().width / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_EXIT_BUTTON),
        boardSprite->getContentSize().height - exitButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_EXITBUTTON)));
    exitButton->addClickEventListener([exitAction](Ref* sender) {
        exitAction();
        });
    boardSprite->addChild(exitButton);

    // Create retry button
    auto retryButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    retryButton->setScale(SpriteController::updateSpriteScale(retryButton, 0.1f));
    retryButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    retryButton->setPosition(Vec2(boardSprite->getContentSize().width / 2, SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    retryButton->addClickEventListener([retryAction](Ref* sender) {
        retryAction();
        });
    boardSprite->addChild(retryButton);

    // Create next scene button
    auto nextSceneButton = ui::Button::create("assets_game/UXUI/Panel/Next_Round_BTN.png", "assets_game/UXUI/Panel/Next_Round_BTN.png");
    nextSceneButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    nextSceneButton->setScale(SpriteController::updateSpriteScale(nextSceneButton, 0.1f));
    nextSceneButton->setPosition(Vec2(retryButton->getPosition().x + SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_PANEL_BUTTON),
        retryButton->getPosition().y));
    nextSceneButton->addClickEventListener([nextSceneAction](Ref* sender) {
        nextSceneAction();
        });
    boardSprite->addChild(nextSceneButton);

    // Create back button
    auto backButton = ui::Button::create("assets_game/UXUI/Panel/Back_BTN.png", "assets_game/UXUI/Panel/Back_BTN_Active.png");
    backButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    backButton->setScale(SpriteController::updateSpriteScale(backButton, 0.1f));
    backButton->setPosition(Vec2(retryButton->getPosition().x - SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_PANEL_BUTTON),
        retryButton->getPosition().y));
    backButton->addClickEventListener([backAction](Ref* sender) {
        backAction();
        });
    boardSprite->addChild(backButton);

    return true;
}

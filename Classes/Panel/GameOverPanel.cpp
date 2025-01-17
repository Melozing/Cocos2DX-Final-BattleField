#include "Panel/GameOverPanel.h"
#include "Button/ReplayButton.h"
#include "Button/ExitButton.h"
#include "ui/CocosGUI.h"
#include "Controller/GameController.h"
#include "Constants/Constants.h"

USING_NS_CC;

GameOverPanel* GameOverPanel::createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction) {
    GameOverPanel* panel = new (std::nothrow) GameOverPanel();
    if (panel && panel->init(retryAction, exitAction, backAction)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool GameOverPanel::init(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction) {
    if (!BasePanel::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    this->removeChild(boardSprite);

    // Create a new board sprite specific to GameOverPanel
    boardSprite = Sprite::create("assets_game/UXUI/Panel/Board_Gameover.png");
    boardSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    boardSprite->setScale(SpriteController::updateSpriteScale(boardSprite, 0.525f));
    boardSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + SpriteController::calculateScreenRatio(0.03f)));
    this->addChild(boardSprite);

    // Create retry button
    auto retryButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
	retryButton->setScale(SpriteController::updateSpriteScale(retryButton, 0.1f));
    retryButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    retryButton->setPosition(Vec2(boardSprite->getContentSize().width/2, SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    retryButton->addClickEventListener([retryAction](Ref* sender) {
        retryAction();
        });
    boardSprite->addChild(retryButton);

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
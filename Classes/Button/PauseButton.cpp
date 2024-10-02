#include "Button/PauseButton.h"
#include "ui/CocosGUI.h"
#include "Game1/Game1Scene.h"
#include "Controller/GameController.h"

USING_NS_CC;

bool PauseButton::init() {
    if (!Node::init()) {
        return false;
    }

    // Create the pause button
    auto pauseButton = ui::Button::create("assets_game/UXUI/Buttons/Pause_BTN.png", "assets_game/UXUI/Buttons/Pause_BTN_Active.png");
    pauseButton->setPosition(Vec2(50, 50)); // Set the position of the pause button
    pauseButton->addClickEventListener([this](Ref* sender) {
        if (!GameController::getInstance()->isGameOver()) { // Check if the game is not over
            this->pauseGame();
        }
        });
    this->addChild(pauseButton, 1); // Set z-order to 1

    // Create the pause panel (initially hidden)
    _pausePanel = ui::Layout::create();
    _pausePanel->setContentSize(Director::getInstance()->getVisibleSize());
    _pausePanel->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _pausePanel->setBackGroundColor(Color3B(0, 0, 0));
    _pausePanel->setBackGroundColorOpacity(150);
    _pausePanel->setVisible(false);
    this->addChild(_pausePanel, 2); // Set z-order to 2

    // Create the continue button
    auto continueButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    continueButton->setPosition(Vec2(_pausePanel->getContentSize().width / 2, _pausePanel->getContentSize().height / 2));
    continueButton->addClickEventListener([this](Ref* sender) {
        this->continueGame();
        });
    _pausePanel->addChild(continueButton);

    return true;
}

void PauseButton::pauseGame() {
    Director::getInstance()->pause();
    _pausePanel->setVisible(true); // Ensure the pause panel is visible
    _paused = true; // Set paused state
}

void PauseButton::continueGame() {
    Director::getInstance()->resume();
    _pausePanel->setVisible(false); // Ensure the pause panel is hidden
    _paused = false; // Reset paused state
}

bool PauseButton::isPaused() const {
    return _paused; // Return the paused state
}

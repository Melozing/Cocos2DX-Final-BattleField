#include "Scene/MainMenuScene.h"
#include "Controller/SceneController.h"
#include "Scene/LoadingScene.h"
#include "Controller/SpriteController.h"
#include "Controller/GameController.h"
#include "Controller/SoundController.h"
#include "Manager/BackgroundManager.h"
#include "Library/ItemLibraryWindow.h" 

USING_NS_CC;

Scene* MainMenu::createScene() {
    return MainMenu::create();
}

bool MainMenu::init() {
    if (!Scene::init()) {
        return false; // Ensure initialization is successful
    }

    // Set Sound
    SoundController::getInstance()->playMusic(Constants::MainMenuTrackPath, true);

    // Set background
    BackgroundManager::getInstance()->setBackgroundWithOverlay(this, "assets_game/UXUI/Background/DienBienPhuVictory.jpg");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto sprite = Sprite::create("assets_game/UXUI/Buttons/Game_1.png");

    // Calculate positions
    float buttonSpacing = SpriteController::calculateScreenHeightRatio(Constants::PADDING_VERTICAL_BUTTONS_MAINMENU); // Adjust this value to set the spacing between buttons
    float buttonY = visibleSize.height / 2;
    float buttonG2Y = buttonY - buttonSpacing;
    float buttonG3Y = buttonG2Y - buttonSpacing;
    float buttonG4Y = buttonG3Y - buttonSpacing;

    // Create and position button
    auto button = ui::Button::create("assets_game/UXUI/Buttons/Game_1.png");
    button->setScale(SpriteController::updateSpriteScale(sprite, 0.08f));
    button->setPosition(Vec2(visibleSize.width / 2, buttonY));
    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
            SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
            startLoading(Constants::GAME1_SCENE_NAME); // Start loading when button is pressed
        }
        });
    this->addChild(button);

    // Check UserDefault value for buttonG2
    bool isButtonG2Active = UserDefault::getInstance()->getBoolForKey(Constants::IS_ACTIVE_GAME2.c_str());

    // Create and position buttonG2
    auto buttonG2 = ui::Button::create(
        isButtonG2Active ? "assets_game/UXUI/Buttons/Game_2_active.png" : "assets_game/UXUI/Buttons/Game_2_inactive.png"
    );
    buttonG2->setScale(SpriteController::updateSpriteScale(sprite, 0.08f));
    buttonG2->setPosition(Vec2(visibleSize.width / 2, buttonG2Y));
    //buttonG2->setEnabled(isButtonG2Active); // Disable button if inactive
    buttonG2->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        //if (type == ui::Widget::TouchEventType::ENDED && isButtonG2Active) {
            //if (isButtonG2Active) {
                SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
                SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
                startLoading(Constants::GAME3_SCENE_NAME); // Start loading when button is pressed
            //}
        //}
        });
    this->addChild(buttonG2);

    // Create and position buttonG3
    auto buttonG3 = ui::Button::create("assets_game/UXUI/Buttons/Game_3.png");
    buttonG3->setScale(SpriteController::updateSpriteScale(sprite, 0.08f));
    buttonG3->setPosition(Vec2(visibleSize.width / 2, buttonG3Y));
    buttonG3->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
            SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
            startLoading(Constants::GAME3_SCENE_NAME); // Start loading when button is pressed
        }
        });
    //this->addChild(buttonG3);

    // Create and position exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Exit_BTN.png");
    exitButton->setScale(SpriteController::updateSpriteScale(exitButton, 0.08f));
    exitButton->setPosition(Vec2(visibleSize.width / 2, buttonG4Y)); // Position at the bottom
    exitButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
            SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
            Director::getInstance()->end(); // Exit the game
        }
        });
    this->addChild(exitButton);

    // Create and add the custom cursor
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/UXUI/Main_Menu/pointer.png");
    _cursor->setAnchorPoint(Vec2(0.5, 0.5));
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f));
    _cursor->setInitialPosition();
    if (_cursor) {
        this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR); // Add cursor to the scene with z-order 1
    }

    // Create and position collection button
    auto collectionButton = ui::Button::create("assets_game/UXUI/Collection/info.png");
    collectionButton->setScale(SpriteController::updateSpriteScale(collectionButton, 0.08f));
    collectionButton->setPosition(Vec2(visibleSize.width - collectionButton->getContentSize().width / 2 - 10, collectionButton->getContentSize().height / 2 + 10)); // Position at the bottom right
    collectionButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            toggleItemLibraryWindow();
        }
        });
    this->addChild(collectionButton);

    return true; // Initialization succeeded
}

void MainMenu::startLoading(std::string nameScene) {
    auto loadingScene = LoadingScene::createScene(nameScene);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, loadingScene));
}

void MainMenu::toggleItemLibraryWindow() {
    if (itemLibraryWindow) {
        itemLibraryWindow->removeFromParent();
        itemLibraryWindow = nullptr;
        overlayLayer->removeFromParent();
        overlayLayer = nullptr;
    }
    else {
        itemLibraryWindow = ItemLibraryWindow::create();
        this->addChild(itemLibraryWindow, 10);
        overlayLayer = LayerColor::create(Color4B(0, 0, 0, 0));
        this->addChild(overlayLayer, 9);
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
            if (itemLibraryWindow && !itemLibraryWindow->getBoundingBox().containsPoint(touch->getLocation())) {
                toggleItemLibraryWindow();
                return true;
            }
            return false;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, overlayLayer);
    }
}

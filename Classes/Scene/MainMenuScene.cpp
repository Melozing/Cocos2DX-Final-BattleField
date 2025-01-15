#include "Scene/MainMenuScene.h"
#include "Controller/SceneController.h"
#include "Scene/LoadingScene.h"
#include "Controller/SpriteController.h"
#include "Controller/GameController.h"
#include "Controller/SoundController.h"
#include "Manager/BackgroundManager.h"
#include "Library/LibraryWindow.h"

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Setup scene elements
    setupBackground();
    setupBoards();
    setupButtons();
    setupCollectionButton();

    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_WINDOWS ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        setupCursor();
    }

    // Register notification listener
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainMenu::onCloseLibraryWindow), "CloseLibraryWindow", nullptr);

    return true; // Initialization succeeded
}

MainMenu::~MainMenu() {
    // Remove notification listener
    NotificationCenter::getInstance()->removeObserver(this, "CloseLibraryWindow");
}

void MainMenu::onCloseLibraryWindow(Ref* sender) {
}

void MainMenu::setupBackground() {
    BackgroundManager::getInstance()->setBackgroundWithOverlay(this, "assets_game/UXUI/Background/background_ui_menu_blur.jpg");
}

void MainMenu::setupBoards() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    // Initialize current board index
    currentBoardIndex = 1;

    // Create and position board1
    board1 = Sprite::create("assets_game/UXUI/Main_Menu/board_game1.png");
	auto scaleBoard = SpriteController::updateSpriteScale(board1, 0.75f);
	board1->setScale(scaleBoard);
    auto sizeScale = SpriteController::GetContentSize(board1);
    board1->setPosition(Vec2(visibleSize.width / 2 + origin.x - sizeScale.width / 1.5f, visibleSize.height / 2 + origin.y));
    this->addChild(board1);

    // Create and position board2
    board2 = Sprite::create("assets_game/UXUI/Main_Menu/board_game2.png");
    board2->setScale(scaleBoard);
    board2->setPosition(Vec2(visibleSize.width / 2 + origin.x - sizeScale.width / 1.5f, visibleSize.height / 2 + origin.y));
    this->addChild(board2);

    // Create and position board3
    board3 = Sprite::create("assets_game/UXUI/Main_Menu/board_game3.png");
    board3->setScale(scaleBoard);
    board3->setPosition(Vec2(visibleSize.width / 2 + origin.x - sizeScale.width / 1.5f, visibleSize.height / 2 + origin.y));
    this->addChild(board3);

    // Initially show only board1
    board1->setVisible(true);
    board2->setVisible(false);
    board3->setVisible(false);
}

void MainMenu::setupButtons() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto sprite = Sprite::create("assets_game/UXUI/Main_Menu/Game_1_BG_BUTTON.png");

    // Calculate positions
    float buttonSpacing = SpriteController::calculateScreenHeightRatio(Constants::PADDING_VERTICAL_BUTTONS_MAINMENU);
    float buttonSpacingHeight = SpriteController::calculateScreenRatio(0.1);
    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        buttonSpacing = SpriteController::calculateScreenRatio(0.13);
        buttonSpacingHeight = SpriteController::calculateScreenRatio(0.13);
    }


    float buttonY = visibleSize.height / 2 + buttonSpacingHeight;
    float buttonG2Y = buttonY - buttonSpacing;
    float buttonG3Y = buttonG2Y - buttonSpacing;
    float buttonG4Y = buttonG3Y - buttonSpacing;

    // Create and position button
    auto button = ui::Button::create("assets_game/UXUI/Main_Menu/Game_1_BG_BUTTON.png");
    button->setScale(SpriteController::updateSpriteScale(sprite, 0.5f));
    button->setPosition(Vec2(visibleSize.width * 0.7, buttonY));
    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            switchBoard(1);
        }
        });
    this->addChild(button);

    // Create and position play button inside button
    auto playButton1 = ui::Button::create("assets_game/UXUI/Main_Menu/Play_BTN.png");
    auto playBtnScale = SpriteController::updateSpriteScale(playButton1, 0.3f);
    auto paddingWidth = SpriteController::calculateScreenRatio(0.025f);

    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        playBtnScale = SpriteController::updateSpriteScale(playButton1, 0.15f);
        paddingWidth = SpriteController::calculateScreenRatio(0.01f);
    }
    playButton1->setScale(playBtnScale);
	auto playBtnSize = SpriteController::GetContentSize(playButton1);
    playButton1->setPosition(Vec2(button->getContentSize().width - playBtnSize.width / 2 - paddingWidth, button->getContentSize().height / 2));
    playButton1->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
            SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
            startLoading(Constants::GAME1_SCENE_NAME); // Start loading when button is pressed
        }
        });
    button->addChild(playButton1);

    // Create and position setting button
    settingButton = ui::Button::create("assets_game/UXUI/Panel/Setting.png");
    settingButton->setScale(SpriteController::updateSpriteScale(sprite, 0.5f));
    settingButton->setPosition(Vec2(button->getPosition().x + SpriteController::GetContentSize(button).width /2.28, buttonY + buttonSpacing / 1.34f));
    settingButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
            showSettingPanel();
        }
        });
    this->addChild(settingButton);


    // Check UserDefault value for buttonG2
    bool isButtonG2Active = UserDefault::getInstance()->getBoolForKey(Constants::IS_ACTIVE_GAME2.c_str());

    // Create and position buttonG2
    auto buttonG2 = ui::Button::create(
        isButtonG2Active ? "assets_game/UXUI/Main_Menu/Game_2_BG_BUTTON.png" : "assets_game/UXUI/Main_Menu/Game_2_BG_BUTTON_INACTIVE.png"
    );
    buttonG2->setScale(SpriteController::updateSpriteScale(sprite, 0.5f));
    buttonG2->setPosition(Vec2(button->getPosition().x, buttonG2Y));
    buttonG2->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            switchBoard(2);
        }
        });
    this->addChild(buttonG2);

    // Create and position play button inside buttonG2
    if (isButtonG2Active) {
        auto playButton2 = ui::Button::create("assets_game/UXUI/Main_Menu/Play_BTN.png");
        playButton2->setScale(playBtnScale);
        playButton2->setPosition(Vec2(buttonG2->getContentSize().width - playBtnSize.width / 2 - paddingWidth, buttonG2->getContentSize().height / 2));
        playButton2->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
                SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
                SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
                startLoading(Constants::GAME3_SCENE_NAME); 
            }
            });
        buttonG2->addChild(playButton2);
    }

    //Create and position buttonG3
    auto buttonG3 = ui::Button::create("assets_game/UXUI/Main_Menu/Game_3_BG_BUTTON.png");
    buttonG3->setScale(SpriteController::updateSpriteScale(sprite, 0.5f));
    buttonG3->setPosition(Vec2(button->getPosition().x, buttonG3Y));
    buttonG3->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            switchBoard(3);
        }
        });
    this->addChild(buttonG3);
}

void MainMenu::setupCursor() {
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/UXUI/Main_Menu/pointer.png");
    _cursor->setAnchorPoint(Vec2(0.5, 0.5));
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f));
    _cursor->setInitialPosition();
    if (_cursor) {
        this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR); // Add cursor to the scene with z-order 1
    }
}

void MainMenu::setupCollectionButton() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto bottomBar = Sprite::create("assets_game/UXUI/Main_Menu/BottomBar.png");
    bottomBar->setScaleX(visibleSize.width / bottomBar->getContentSize().width);
    bottomBar->setScaleY(SpriteController::updateSpriteScale(bottomBar,1.0f));
    bottomBar->setPosition(Vec2(visibleSize.width / 2, SpriteController::GetContentSize(bottomBar).height / 2));
    this->addChild(bottomBar);

	float buttonSpacing = SpriteController::calculateScreenRatio(0.01f);

    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        buttonSpacing = SpriteController::calculateScreenHeightRatio(0.01f);
    }

    collectionButton = ui::Button::create("assets_game/UXUI/Collection/info.png");
    collectionButton->setScale(SpriteController::updateSpriteScale(collectionButton, 0.1f));
    collectionButton->setAnchorPoint(Vec2(0.5,0.5));
    collectionButton->setPosition(Vec2(visibleSize.width / 2, bottomBar->getPositionY() + buttonSpacing));
    collectionButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
            itemLibraryWindow->toggleItemLibraryWindow(collectionButton->getPosition());
        }
        });
    this->addChild(collectionButton);
    createItemLibrary();
}

void MainMenu::startLoading(std::string nameScene) {
    auto loadingScene = LoadingScene::createScene(nameScene);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, loadingScene));
}

void MainMenu::createItemLibrary() {
    itemLibraryWindow = LibraryWindow::create();
    itemLibraryWindow->setVisible(false);
    itemLibraryWindow->setPosition(collectionButton->getPosition());
    this->addChild(itemLibraryWindow, Constants::ORDER_LAYER_LAYOUT_UI + 10);
}

void MainMenu::switchBoard(int boardIndex) {
    if (currentBoardIndex == boardIndex) {
        return; // Do nothing if the requested board is already visible
    }

    SoundController::getInstance()->playSoundEffect(Constants::ClickButtonGameSFX);
    board1->setVisible(boardIndex == 1);
    board2->setVisible(boardIndex == 2);
    board3->setVisible(boardIndex == 3);

    // Update current board index
    currentBoardIndex = boardIndex;
}

void MainMenu::showSettingPanel() {
    if (!settingPanel) {
        settingPanel = SettingPanel::createPanel([this]() {
            hideSettingPanel();
            }, []() {
                Director::getInstance()->end(); // Quit game
                });
        this->addChild(settingPanel);
    }
    settingPanel->setVisible(true);
}

void MainMenu::hideSettingPanel() {
    if (settingPanel) {
        settingPanel->setVisible(false);
    }
}
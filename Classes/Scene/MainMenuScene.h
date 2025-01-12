#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Game2/Cursor/Cursor.h"
#include "Panel/SettingPanel.h"
#include "Library/ItemLibraryWindow.h"

USING_NS_CC;

class MainMenu : public Scene {
public:
    // Scene creation
    static Scene* createScene();

    // Initialization
    virtual bool init() override;

    // Macro to create the MainMenu instance
    CREATE_FUNC(MainMenu);

    // Toggle the item library window
    void toggleItemLibraryWindow();

private:
    // Method to start loading the next scene
    void startLoading(std::string nameScene);

    // Cursor instance
    Cursor* _cursor;

    // UI elements
    cocos2d::ui::Button* collectionButton;
    cocos2d::ui::Button* settingButton;
    ItemLibraryWindow* itemLibraryWindow;
    cocos2d::LayerColor* overlayLayer;

    // Boards
    int currentBoardIndex;
    cocos2d::Sprite* board1;
    cocos2d::Sprite* board2;
    cocos2d::Sprite* board3;

    // Helper methods
    void setupBackground();
    void setupButtons();
    void setupBoards();
    void setupCursor();
    void setupCollectionButton();
    void switchBoard(int boardIndex);

    // Setting Panel
    SettingPanel* settingPanel;

    void showSettingPanel();
    void hideSettingPanel();
};

#endif // __MAIN_MENU_SCENE_H__

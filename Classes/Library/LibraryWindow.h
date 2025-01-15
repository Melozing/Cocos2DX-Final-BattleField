#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LibraryWindow : public cocos2d::Node
{
public:
    virtual bool init();
    CREATE_FUNC(LibraryWindow);

    // Toggle the item library window
    void toggleItemLibraryWindow(const cocos2d::Vec2& position);
    
    struct ItemData {
        std::string iconPath;
        std::string name;
        std::string imagePath;
        std::string content;
        bool isActive = false; 
        cocos2d::Sprite* activeBackground;
        cocos2d::Sprite* inactiveBackground;
    };

private:
    void initBackground();
    void initCloseButton();
    void addLeftInfoImage();
    void addRightInfoImage();
    void addTopCenterLabel();
    void setupOverlay();

    cocos2d::Sprite* backgroundImage;
    bool isItemLibraryVisible = false;
    cocos2d::ui::Button* closeButton;
    cocos2d::Sprite* leftInfoImage;
    cocos2d::Sprite* rightInfoImage;
    cocos2d::Sprite* paperSprite;
    cocos2d::Sprite* paperSpriteFront;
    cocos2d::Label* topCenterLabel;
    cocos2d::Sprite* overlayLayer;

    // View item
    void loadItemData(const std::string& filePath);
    void setupScollItems();
    void displayItemDetails(size_t index);
    void updateItemStates(); 
    cocos2d::ui::ScrollView* itemScrollView;
    std::vector<ItemData> items;
    std::vector<cocos2d::ui::Button*> itemButtons; 
};

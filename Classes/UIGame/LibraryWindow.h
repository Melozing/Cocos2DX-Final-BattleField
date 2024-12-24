#ifndef __LIBRARY_WINDOW_H__
#define __LIBRARY_WINDOW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include <string>
#include "json/document.h"

struct ItemData {
    int id;
    std::string imagePath;
    std::string name;
    std::string description;
};

class ItemWidget : public cocos2d::ui::Widget {
public:
    static ItemWidget* create(const ItemData& itemData);
    bool init(const ItemData& itemData);
    void updateItem(const ItemData& itemData);

private:
    cocos2d::Sprite* itemBg;
    cocos2d::ui::Button* itemButton;
    cocos2d::Label* itemNameLabel;
};

class LibraryWindow : public cocos2d::Layer {
public:
    virtual bool init();
    CREATE_FUNC(LibraryWindow);
    void updateItemInfo(const ItemData& item);
private:
    cocos2d::Sprite* mainBg;
    cocos2d::ui::ScrollView* scrollView;
    cocos2d::Label* itemInfoLabel;
    cocos2d::ui::ScrollView* infoScrollView;
    std::vector<ItemData> items;
    void createBackground();
    void createItemSection();
    void createInfoSection();
    void createExitButton();
    void loadItemsFromJson();
};

#endif // __LIBRARY_WINDOW_H__
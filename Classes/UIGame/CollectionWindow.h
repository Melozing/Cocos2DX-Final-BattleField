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

class LibraryWindow : public cocos2d::Layer {
public:
    virtual bool init();
    CREATE_FUNC(LibraryWindow);

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
    void updateItemInfo(const ItemData& item);
};

#endif // __LIBRARY_WINDOW_H__

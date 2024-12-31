#ifndef __ITEM_LIBRARY_WINDOW_H__
#define __ITEM_LIBRARY_WINDOW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Library/ItemLibrary.h"
#include "Library/UILibrary.h"

class ItemLibraryWindow : public cocos2d::Layer {
public:
    virtual bool init();
    CREATE_FUNC(ItemLibraryWindow);
    cocos2d::Rect getMainBgBoundingBox() const;
private:
    void createBackground();
    void createItemSection();
    void createInfoSection();
    void createExitButton();
    void updateItemInfo(const ItemData& item);

    cocos2d::Sprite* mainBg;
    cocos2d::ui::ScrollView* scrollView;
    cocos2d::Label* itemInfoLabel;
    cocos2d::ui::ScrollView* infoScrollView;
    cocos2d::Sprite* itemInfoImage;
};

#endif // __ITEM_LIBRARY_WINDOW_H__


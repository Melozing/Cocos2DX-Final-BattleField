#ifndef __UI_LIBRARY_H__
#define __UI_LIBRARY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Library/ItemLibrary.h"

class UILibrary {
public:
    static cocos2d::ui::Button* createItemButton(const ItemData& itemData, const std::function<void()>& callback);
    static cocos2d::ui::Layout* createItemLayout(const ItemData& itemData, const std::function<void()>& callback);
    static cocos2d::ui::ScrollView* createInfoScrollView(const cocos2d::Size& size);
};

#endif // __UI_LIBRARY_H__


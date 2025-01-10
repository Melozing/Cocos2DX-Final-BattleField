#ifndef __UI_LIBRARY_H__
#define __UI_LIBRARY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Library/ItemLibrary.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"
class UILibrary {
public:
    static cocos2d::ui::Button* createItemButton(const ItemData& itemData, const std::function<void()>& callback);
    static cocos2d::ui::Layout* createItemLayout(const ItemData& itemData, const std::function<void()>& callback);
    static cocos2d::ui::ScrollView* createScrollView(const cocos2d::Size& size, bool custom = false);
};

#endif // __UI_LIBRARY_H__


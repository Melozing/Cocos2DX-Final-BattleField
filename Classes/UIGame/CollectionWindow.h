#ifndef __COLLECTION_WINDOW_H__
#define __COLLECTION_WINDOW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class CollectionWindow : public cocos2d::Layer {
public:
    static CollectionWindow* create();
    virtual bool init();

private:
    void createMainWindow();
    void createLayer1();
    void createLayer2();
    void closeWindow(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onItemClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    cocos2d::ui::ScrollView* scrollView;
    cocos2d::Label* itemInfoLabel;
};

#endif // __COLLECTION_WINDOW_H__

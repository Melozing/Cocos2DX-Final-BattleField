#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class MainMenu : public Scene
{
public:
    static Scene* createScene();

    bool OnPhysicContact(cocos2d::PhysicsContact &contact);

    // Implement the "init" method manually
    virtual bool init() override;

    // Touch event handlers
    bool onTouchBegan(Touch* touch, Event* unused_event);
    bool onTouchMoved(Touch* touch, Event* unused_event);
    bool onTouchEnded(Touch* touch, Event* unused_event);

    // Animation handling
    Animation* createAnimation(const std::string& prefixName, int frameCount, float delay);
    void changeAnimation(const std::string& namePrefix, int numOrder);
    void defaultStateAnimChar();

    // Close callback
    void menuCloseCallback(Ref* pSender);
    cocos2d::ui::LoadingBar* loadingBar;
    void startLoading();

    // Additional methods
    bool sayHello(Touch* touch, Event* unused_event);

    CREATE_FUNC(MainMenu);

private:
    // Physics world
    PhysicsWorld* world;
    void setPhysicsWorld(PhysicsWorld* m_world) { world = m_world; }

    // Sprite and animation
    Sprite* modelCharac;
    Animation* animation;
};

#endif // __MAINMENU_SCENE_H__

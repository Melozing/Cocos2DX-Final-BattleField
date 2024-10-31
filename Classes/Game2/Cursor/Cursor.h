#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "cocos2d.h"

class Cursor : public cocos2d::Sprite {
public:
    Cursor();
    static Cursor* create(const std::string& filename);
    void initMouseListener();
    void setInitialPosition();
    void changeSprite(const std::string& filename);
    void changeColor(const cocos2d::Color3B& color);

private:
    void onMouseMove(cocos2d::Event* event);
    void updateCursorPosition();

    cocos2d::Vec2 _mousePos;
    bool _isUpdating;
};

#endif // __CURSOR_H__

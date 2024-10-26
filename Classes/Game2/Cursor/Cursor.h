#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "cocos2d.h"

class Cursor : public cocos2d::Sprite
{
public:
    Cursor();
    static Cursor* create(const std::string& filename);
    void updateCursorPosition();
    void changeSprite(const std::string& filename);
    bool _isUpdating;
private:
    void initMouseListener();
    void onMouseMove(cocos2d::Event* event);

    cocos2d::Vec2 _mousePos;
};

#endif // __CURSOR_H__
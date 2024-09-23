#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "cocos2d.h"

class Background : public cocos2d::Node
{
public:
    static Background* createBackground(const std::string& filePath, float scrollSpeed);

    virtual bool init(const std::string& filePath, float scrollSpeed);

    void update(float delta);

private:
    cocos2d::Sprite* background1;
    cocos2d::Sprite* background2;
    float scrollSpeed;
};

#endif // __BACKGROUND_H__

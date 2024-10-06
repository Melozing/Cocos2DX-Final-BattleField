#ifndef __BACKGROUND3H__
#define __BACKGROUND3H__

#include "cocos2d.h"

class Background3 : public cocos2d::Node
{
public:
    static Background3* createBackground(const std::string& filePath);

    virtual bool init(const std::string& filePath);


private:
    cocos2d::Sprite* background;

};

#endif // __BACKGROUND_H__

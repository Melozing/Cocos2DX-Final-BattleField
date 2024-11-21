#ifndef __CUSTOM_LOADING_BAR_H__
#define __CUSTOM_LOADING_BAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class CustomLoadingBar : public Node {
public:
    static CustomLoadingBar* create(const std::string& loadingBarImage, const std::string& borderImage, float scale);

    void setPercent(float percent);
    float getPercent() const;
    ui::LoadingBar* getLoadingBar() const;
    cocos2d::Sprite* getBorder() const const;

    void setLoadingBarPosition(const Vec2& position);
    void setBorderPosition(const Vec2& position);
    void setLoadingBarRotation(float rotation);
    void setBorderRotation(float rotation);
    void setLoadingBarScale(float scale);
    void setBorderScale(float scale);

private:
    bool init(const std::string& loadingBarImage, const std::string& borderImage, float scale);

    ui::LoadingBar* loadingBar;
    Sprite* border;
};

#endif // __CUSTOM_LOADING_BAR_H__

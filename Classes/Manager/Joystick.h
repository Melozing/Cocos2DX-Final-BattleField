#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"
    class Joystick : public cocos2d::Node {
    public:
        static Joystick* create(const std::string& baseImage, const std::string& thumbImage);
        virtual bool init(const std::string& baseImage, const std::string& thumbImage);

        std::function<void(const cocos2d::Vec2&)> onDirectionChanged;
        void setJoystickScale(float scale);
        void setPosition(const cocos2d::Vec2& position); 
        void setEnabled(bool enabled);
        bool getEnabled();

    private:
        cocos2d::Sprite* _base;
        cocos2d::Sprite* _thumb;
        cocos2d::Vec2 _direction;
        bool _enabled;

        bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
        void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
        void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    };

#endif // __JOYSTICK_H__

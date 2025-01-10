#include "Manager/Joystick.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"

USING_NS_CC;

Joystick* Joystick::create(const std::string& baseImage, const std::string& thumbImage) {
    Joystick* ret = new (std::nothrow) Joystick();
        
    if (ret && ret->init(baseImage, thumbImage)) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
            return nullptr;
        }
    }

    bool Joystick::init(const std::string& baseImage, const std::string& thumbImage) {
        if (!Node::init()) {
            return false;
        }

        this->setAnchorPoint(Vec2(0.5f, 0.5f));

        _base = cocos2d::Sprite::create(baseImage);
        _thumb = cocos2d::Sprite::create(thumbImage);
        _enabled = true;

        if (!_base || !_thumb) {
            return false;
        }

        this->addChild(_base);
        this->addChild(_thumb);
        this->setVisible(false);

        auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(Joystick::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(Joystick::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(Joystick::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

        return true;
    }

    bool Joystick::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!_enabled) return false;
        if (_base->getBoundingBox().containsPoint(touch->getLocation())) {
            this->setVisible(true);
            _thumb->setPosition(touch->getLocation());
            _direction = (_thumb->getPosition() - _base->getPosition()).getNormalized();
            if (onDirectionChanged) {
                onDirectionChanged(_direction);
            }
            return true;
        }
        return false;
    }

    void Joystick::setJoystickScale(float scale) {
        if (_base) {
            _base->setScale(scale);
        }
        if (_thumb) {
            _thumb->setScale(scale);
        }
    }

    void Joystick::setPosition(const cocos2d::Vec2& position) {
        if (_base) {
            _base->setPosition(position);
        }
        if (_thumb) {
            _thumb->setPosition(position);
        }
    }

    void Joystick::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!_enabled) return;
        Vec2 touchLocation = touch->getLocation();
        Vec2 basePosition = _base->getPosition();
        float maxRadius = SpriteController::GetContentSizeSprite(_base).width / 2;

        Vec2 direction = touchLocation - basePosition;
        if (direction.length() > maxRadius) {
            direction.normalize();
            direction *= maxRadius;
        }

        _thumb->setPosition(basePosition + direction);
        _direction = direction.getNormalized();

        if (direction.length() < SpriteController::calculateScreenRatio(0.015f)) {
            _direction = Vec2::ZERO;
        }

        if (onDirectionChanged) {
            onDirectionChanged(_direction);
        }
    }

    void Joystick::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!_enabled) return;
        this->setVisible(false);
        _thumb->setPosition(_base->getPosition());
        _direction = Vec2::ZERO;
        if (onDirectionChanged) {
            onDirectionChanged(_direction);
        }
    }

    void Joystick::setEnabled(bool enabled) {
        _enabled = enabled;
    }

    bool Joystick::getEnabled() {
        return _enabled;
    }
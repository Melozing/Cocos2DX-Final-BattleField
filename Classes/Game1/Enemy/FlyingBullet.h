#ifndef __FLYING_BULLET_H__
#define __FLYING_BULLET_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Controller/SpriteController.h"

class FlyingBullet : public Enemy, public SpriteController {
public:
    static FlyingBullet* create();

    virtual bool init() override;
    virtual void spawn(const cocos2d::Vec2& startPosition) override;
    virtual void update(float delta) override;

    float getSpeed() const { return _speed; }
    void initAnimation(bool fromLeft);
    Size GetSize();

    virtual ~FlyingBullet();

private:
    cocos2d::Vec2 _direction; // Direction vector
    float _speed;
    bool _fromLeft;
    std::string pngFile;
    bool isLoaded = false;

    cocos2d::Sprite* modelCharac;
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::SpriteBatchNode* spriteBatchNodeLeft;
    cocos2d::SpriteBatchNode* spriteBatchNodeRight;
};

#endif // __FLYING_BULLET_H__
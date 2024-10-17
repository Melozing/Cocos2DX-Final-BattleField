﻿#include "Grenade/Grenade.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

Grenade* Grenade::createGrenade(const Vec2& startPosition, const Vec2& direction, float throwDuration)
{
    Grenade* grenade = new (std::nothrow) Grenade();
    if (grenade && grenade->init(startPosition, direction, throwDuration))
    {
        grenade->autorelease();
        return grenade;
    }
    CC_SAFE_DELETE(grenade);
    return nullptr;
}

bool Grenade::init(const Vec2& startPosition, const Vec2& direction, float throwDuration)
{
    if (!Sprite::initWithFile("assets_game/player/grenade.png"))
    {
        return false;
    }

    this->setPosition(startPosition);
    _direction = direction.getNormalized();
    _throwDuration = throwDuration;

    // Tải explosion.plist vào SpriteFrameCache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/effects/explosion.plist");

    float throwDistance = calculateThrowDistance(_throwDuration);

    auto rotate = RotateBy::create(throwDuration, 360); // Xoay 360 độ
    auto moveBy = MoveBy::create(throwDuration, _direction * throwDistance);
    auto spawn = Spawn::create(rotate, moveBy, nullptr);

    this->runAction(Sequence::create(spawn, CallFunc::create([this]() {
        // Sau khi ném xong, lựu đạn sẽ tồn tại trên mặt đất và nhấp nháy
        this->createImpactEffect();
        this->scheduleOnce([this](float) {
            if (this->getParent() != nullptr) {
                this->explode();
            }
            }, 2.0f, "explode_key"); // Thời gian nhấp nháy trước khi nổ
        }), nullptr));

    return true;
}

void Grenade::createImpactEffect()
{
    // Tạo hiệu ứng khi lựu đạn rơi xuống
    auto impactSprite = Sprite::create("assets_game/effects/warning.png");
    impactSprite->setPosition(this->getPosition());
    this->getParent()->addChild(impactSprite);

    // Chạy animation nhấp nháy cho impactSprite
    auto blink = Blink::create(2.0f, 7); // Nhấp nháy trong 2.0 giây, 7 lần
    impactSprite->runAction(Sequence::create(blink, CallFunc::create([impactSprite]() {
        impactSprite->removeFromParent();
        }), nullptr));
}

void Grenade::explode()
{
    // Tạo Animation cho vụ nổ từ explosion.plist
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 6; ++i)
    {
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(
            StringUtils::format("explosion%d.png", i)
        );
        if (frame == nullptr) {
            CCLOG("Frame %d not found in explosion.plist", i);
            return;
        }
        frames.pushBack(frame);
    }

    auto animation = Animation::createWithSpriteFrames(frames, 0.1f); // Thời gian giữa các khung hình
    auto animate = Animate::create(animation);
    auto repeatAnimate = Repeat::create(animate, 1); // Lặp lại animation 3 lần

    // Tạo Sprite để hiển thị Animation
    auto explosionSprite = Sprite::createWithSpriteFrame(frames.front());
    explosionSprite->setPosition(this->getPosition());
    this->getParent()->addChild(explosionSprite);

    // Xóa hình ảnh lựu đạn
    this->setVisible(false);

    explosionSprite->runAction(Sequence::create(repeatAnimate, CallFunc::create([explosionSprite, this]() {
        explosionSprite->removeFromParent();
        this->removeFromParent();
        }), nullptr));

    // Gây sát thương
    this->dealDamage();

    // Logic gây sát thương ở đây
    // ...
}

void Grenade::dealDamage()
{
    // Tạo một hình tròn đại diện cho vùng gây sát thương
    float damageRadius = 100.0f; // Bán kính gây sát thương
    auto damageCircle = DrawNode::create();
    damageCircle->drawSolidCircle(this->getPosition(), damageRadius, 0, 50, Color4F(1, 0, 0, 0.5f));
    this->getParent()->addChild(damageCircle);

    // Kiểm tra va chạm với các đối tượng khác trong game
    auto children = this->getParent()->getChildren();
    for (auto child : children)
    {
        if (child == this) continue;

        // Giả sử các đối tượng khác có phương thức getBoundingBox()
        if (child->getBoundingBox().intersectsCircle(this->getPosition(), damageRadius))
        {
            // Gây sát thương cho đối tượng
            // Giả sử các đối tượng khác có phương thức takeDamage(int damage)
            //child->takeDamage(50); // Gây 50 sát thương
        }
    }

    // Xóa hình tròn sau khi gây sát thương
    damageCircle->removeFromParent();
}

float Grenade::calculateThrowDistance(float holdTime)
{
    const float maxHoldTime = 2.0f; // Thời gian giữ chuột tối đa
    const float minThrowDistance = 100.0f; // Khoảng cách ném tối thiểu
    const float maxThrowDistance = Constants::GrenadeThrowDistance; // Khoảng cách ném tối đa
    float normalizedHoldTime = fmod(holdTime, maxHoldTime * 2.0f); // Chu kỳ tăng giảm lực

    if (normalizedHoldTime > maxHoldTime)
    {
        normalizedHoldTime = maxHoldTime * 2.0f - normalizedHoldTime;
    }

    return minThrowDistance + (maxThrowDistance - minThrowDistance) * (normalizedHoldTime / maxHoldTime);
}
// Grenade.cpp
#include "Grenade/Grenade.h"
#include "Constants/Constants.h"

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

    float throwDistance = calculateThrowDistance(_throwDuration);

    auto rotate = RotateBy::create(throwDuration, 360); // Xoay 360 độ
    auto moveBy = MoveBy::create(throwDuration, _direction * throwDistance);
    auto spawn = Spawn::create(rotate, moveBy, nullptr);

    this->runAction(Sequence::create(spawn, CallFunc::create([this]() {
        // Sau khi ném xong, lựu đạn sẽ tồn tại trên mặt đất
        this->scheduleOnce([this](float) {
            this->explode();
            }, 1.5f, "explode_key");
        }), nullptr));

    return true;
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
        frames.pushBack(frame);
    }

    auto animation = Animation::createWithSpriteFrames(frames, 0.1f); // Thời gian giữa các khung hình
    auto animate = Animate::create(animation);

    // Tạo Sprite để hiển thị Animation
    auto explosionSprite = Sprite::createWithSpriteFrame(frames.front());
    explosionSprite->setPosition(this->getPosition());
    this->getParent()->addChild(explosionSprite);

    // Xóa hình ảnh lựu đạn
    this->setVisible(false);

    explosionSprite->runAction(Sequence::create(animate, CallFunc::create([explosionSprite, this]() {
        explosionSprite->removeFromParent();
        this->removeFromParent();
        }), nullptr));

    // Logic gây sát thương ở đây
    // ...
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

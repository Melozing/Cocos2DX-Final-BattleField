#include "Game2/Player/PlayerGame2.h"
#include "cocos2d.h"

USING_NS_CC;

PlayerGame2::PlayerGame2() : _mousePos(Vec2::ZERO), _velocity(Vec2::ZERO), _speed(100.0f), _isMoving(false) {}
PlayerGame2::~PlayerGame2() {}

bool PlayerGame2::init()
{
    // Load the sprite frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/maingame.plist");

    // Check if the sprite frame exists in the cache
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Walk_riffle_000.png");
    if (!spriteFrame)
    {
        CCLOG("Sprite frame 'Walk_riffle_000.png' not found in the cache");
        return false;
    }

    // Initialize the sprite with the sprite frame
    if (!Sprite::initWithSpriteFrame(spriteFrame))
    {
        return false;
    }
    
    this->setPosition(Vec2(100, 100));
    this->setScale(0.25f);
    this->setAnchorPoint(Vec2(0.5, 0.5)); // Set anchor point at the head of the character

    // Add mouse event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PlayerGame2::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame2::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Schedule update method
    this->scheduleUpdate();

    // Create animation
    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 10; i++) // Assuming you have 10 frames for the walk animation
    {
        std::string frameName = "Walk_riffle_" + StringUtils::format("%03d", i) + ".png";
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
        else
        {
            CCLOG("Frame %s not found", frameName.c_str());
        }
    }

    if (!animFrames.empty())
    {
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        _animate = Animate::create(animation);
        _animate->retain();
    }
    else
    {
        CCLOG("No frames found for animation");
    }

    return true;
}

PlayerGame2* PlayerGame2::createPlayerGame2()
{
    PlayerGame2* Player = new (std::nothrow) PlayerGame2();
    if (Player && Player->init())
    {
        Player->autorelease();
        return Player;
    }
    else
    {
        delete Player;
        return nullptr;
    }
}

void PlayerGame2::onMouseMove(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());
}

void PlayerGame2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_W:
        moveUp();
        break;
    case EventKeyboard::KeyCode::KEY_S:
        moveDown();
        break;
    case EventKeyboard::KeyCode::KEY_A:
        moveLeft();
        break;
    case EventKeyboard::KeyCode::KEY_D:
        moveRight();
        break;
    default:
        break;
    }
}

void PlayerGame2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_S:
        stopVerticalMovement();
        break;
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
        stopHorizontalMovement();
        break;
    default:
        break;
    }
}

void PlayerGame2::moveUp()
{
    _velocity.y = 1;
    _isMoving = true;
}

void PlayerGame2::moveDown()
{
    _velocity.y = -1;
    _isMoving = true;
}

void PlayerGame2::moveLeft()
{
    _velocity.x = -1;
    _isMoving = true;
}

void PlayerGame2::moveRight()
{
    _velocity.x = 1;
    _isMoving = true;
}

void PlayerGame2::stopVerticalMovement()
{
    _velocity.y = 0;
    if (_velocity.x == 0)
    {
        _isMoving = false;
    }
}

void PlayerGame2::stopHorizontalMovement()
{
    _velocity.x = 0;
    if (_velocity.y == 0)
    {
        _isMoving = false;
    }
}

void PlayerGame2::update(float delta)
{
    // Cập nhật vị trí dựa trên vận tốc
    Vec2 position = this->getPosition();
    position += _velocity * _speed * delta;
    this->setPosition(position);

    // Chuyển đổi vị trí chuột từ hệ tọa độ màn hình sang hệ tọa độ OpenGL
    auto mousePos = Director::getInstance()->convertToGL(_mousePos);
    Vec2 PlayerGame2Pos = this->getPosition();
    Vec2 direction = mousePos - PlayerGame2Pos;

    // Tính toán góc quay từ hướng
    float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));

    // Điều chỉnh góc quay để đảm bảo nhân vật luôn hướng về phía chuột
    this->setRotation(-angle + 90);

    // Lật hình ảnh nếu cần thiết
    if (angle > 90 || angle < -90)
    {
        this->setFlippedY(true);
    }
    else
    {
        this->setFlippedY(false);
    }

    // Chạy hoạt ảnh khi di chuyển
    if (_isMoving)
    {
        if (!this->getActionByTag(1))
        {
            auto animateAction = RepeatForever::create(_animate);
            animateAction->setTag(1);
            this->runAction(animateAction);
        }
    }
    else
    {
        this->stopActionByTag(1);
    }
}






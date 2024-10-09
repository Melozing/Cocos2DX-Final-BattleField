#include "PlayerGame2.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "cocos2d.h"
#include "Bullet/Bullet.h"

USING_NS_CC;

PlayerGame2::PlayerGame2()
    : _mousePos(Vec2::ZERO),
    _isMouseDown(false),
    modelCharac(nullptr),
    bulletManager(nullptr)
{
}

PlayerGame2::~PlayerGame2()
{
    delete bulletManager;
}

PlayerGame2* PlayerGame2::createPlayerGame2()
{
    PlayerGame2* player = new (std::nothrow) PlayerGame2();
    player->cocos2d::Sprite::setName("PlayerGame2");
    if (player && player->init())
    {
        player->autorelease();
        player->initAnimation();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool PlayerGame2::init() {
    if (!cocos2d::Sprite::init()) {
        return false;
    }

    this->cocos2d::Sprite::setPosition(Vec2(Constants::InitialPosX, Constants::InitialPosY));
    this->cocos2d::Sprite::setScale(Constants::PlayerScale);
    this->cocos2d::Sprite::setAnchorPoint(Vec2(0.5, 0.5)); // Set anchor point at the head of the character

    auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setGravityEnable(false); // Disable gravity
    this->cocos2d::Sprite::setPhysicsBody(physicsBody);

    // Add mouse event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PlayerGame2::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(PlayerGame2::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(PlayerGame2::onMouseUp, this);
    cocos2d::Sprite::_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame2::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame2::onKeyReleased, this);
    cocos2d::Sprite::_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Schedule update method
    this->scheduleUpdate();

    // Initialize BulletManager
    bulletManager = new BulletManager(100, "assets_game/player/shot.png");

    return true;
}

void PlayerGame2::initAnimation()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/walkriffle.plist");

    auto spriteBatchNode = SpriteBatchNode::create("assets_game/player/walkriffle.png");
    this->addChild(spriteBatchNode);

    modelCharac = Sprite::createWithSpriteFrameName("walkriffle0.png");
    SpriteController::updateSpriteScale(modelCharac, 0.05f);
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.25f));

    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation("walkriffle", 5, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void PlayerGame2::startMovementAnimation()
{
    if (!this->getActionByTag(1))
    {
        auto animateCharac = Animate::create(createAnimation("walkriffle", 5, 0.07f));
        auto repeatAnimate = RepeatForever::create(animateCharac);
        repeatAnimate->setTag(1);
        modelCharac->runAction(repeatAnimate);
    }
}

void PlayerGame2::onMouseMove(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // Invert the y-coordinate
    auto winSize = Director::getInstance()->getWinSize();
    _mousePos.y = winSize.height - _mousePos.y;
}

void PlayerGame2::onMouseDown(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT || e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        _isMouseDown = true; // Mark that the mouse is down
    }
}

void PlayerGame2::onMouseUp(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if ((e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT || e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) && _isMouseDown)
    {
        // Convert mouse position to OpenGL coordinates
        auto mousePos = Director::getInstance()->convertToGL(_mousePos);
        Vec2 pos = this->getPosition();

        // Calculate shooting direction
        Vec2 dirToShoot = mousePos - pos;

        // Shoot the bullet
        shootBullet(dirToShoot);

        // Reset mouse state
        _isMouseDown = false;
    }
}

void PlayerGame2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    PlayerMovement::onKeyPressed(keyCode, event);
}

void PlayerGame2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    PlayerMovement::onKeyReleased(keyCode, event);
}


void PlayerGame2::update(float delta)
{
    PlayerMovement::update(delta);
    RotateToMouse();
    if (_isMoving)
    {
        startMovementAnimation();
    }
    else
    {
        modelCharac->stopActionByTag(1);
    }
}

void PlayerGame2::RotateToMouse()
{
    auto mousePos = Director::getInstance()->convertToGL(_mousePos);
    Vec2 pos = this->getPosition();
    Vec2 dirToFace = mousePos - pos;
    dirToFace.normalize();
    float angle = MathFunction::GetDirInDegreesPositive(dirToFace);
    this->setRotation(-angle + 90);
}

void PlayerGame2::shootBullet(const Vec2& direction)
{
    CCLOG("shootBullet called");

    Vec2 normalizedDirection = direction.getNormalized();
    bulletManager->SpawnBullet(this->getPosition(), normalizedDirection, Constants::BulletSpeed);
}

void PlayerGame2::die()
{
    this->removeFromParent();
    // Add any additional logic for player death, such as game over screen
}

#include "MainMenuScene.h"
#include <string>
#include "cocos2d.h"
#include "Game1/Game1Scene.h"
#include "Game2/Game2Scene.h"
#include "ui/CocosGUI.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = MainMenu::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    CCLOG("Error while loading: %s", filename);
    CCLOG("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp");
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto button = ui::Button::create("assets_game/UXUI/Main_Menu/Start_BTN.png");
    auto buttonSprite = cocos2d::Sprite::create("assets_game/UXUI/Main_Menu/Start_BTN.png");
    button->setScale(SpriteController::updateSpriteScale(buttonSprite, 0.1f));
    button->setAnchorPoint(Vec2(0.5f, 0.5f));
    button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        startLoading();
        });
    this->addChild(button);
    auto loadSprite = cocos2d::Sprite::create("assets_game/UXUI/Loading/Loading_Bar_A.png");
    loadingBar = ui::LoadingBar::create("assets_game/UXUI/Loading/Loading_Bar_A.png");
    loadingBar->setScale(SpriteController::updateSpriteScale(loadSprite, 0.6f));
    loadingBar->setPercent(0); // Set initial percentage to 0%
    loadingBar->setAnchorPoint(Vec2(0.5f, 0.5f));
    loadingBar->setDirection(ui::LoadingBar::Direction::LEFT); // Set direction of the loading bar
    loadingBar->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + loadingBar->getContentSize().height + SpriteController::calculateScreenRatio(0.05f)));
    // Position it in the center
    this->addChild(loadingBar);

    auto colListener = EventListenerPhysicsContact::create();
    colListener->onContactBegin = CC_CALLBACK_1(MainMenu::OnPhysicContact, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(colListener, this);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(MainMenu::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(MainMenu::onTouchEnded, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(MainMenu::onTouchMoved, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void MainMenu::startLoading() {
    // Create and set up the border for the Loading Bar
    auto border = Sprite::create("assets_game/UXUI/Loading/Loading_Bar_Border.png");
    auto loadingPos = loadingBar->getPosition();
    border->setAnchorPoint(Vec2(0.5f, 0.5f));
    border->setScale(loadingBar->getScale());
    float loadingBarHeight = loadingBar->getContentSize().height * loadingBar->getScaleY() + loadingBar->getScaleY();
    loadingPos.y += loadingBarHeight;
    border->setPosition(loadingPos); // Align with the loading bar
    this->addChild(border, -1); // Place behind the loading bar

    // Smoothly increase the loading bar's percentage
    this->schedule([this](float dt) {
        float percent = loadingBar->getPercent();
        percent += 0.5f; // Increase by 0.5 for smoothness

        // Update the LoadingBar percentage
        loadingBar->setPercent(percent);

        // If the loading reaches 100%, stop the schedule and transition to the next scene
        if (percent >= 100) {
            loadingBar->setPercent(100); // Ensure it stays at 100%
            this->unschedule("loading_bar_update_key");

            // Delay before switching scenes
            this->runAction(Sequence::create(
                CallFunc::create([]() {
                    CCLOG("Loading complete, transitioning to Game2Scene");
                    auto gameScene = Game1Scene::createScene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameScene));
                    }),
                nullptr
            ));
        }
        }, 0.2f / 60.0f, "loading_bar_update_key"); // Update every frame
}


bool MainMenu::onTouchBegan(Touch* touch, Event* unused_event)
{
    //changeAnimation("Jump", 11);
    return true;
}

void MainMenu::changeAnimation(const std::string& namePrefix, int numOrder)
{
    animation = createAnimation(namePrefix, numOrder, 0.035f);
    auto animate = Animate::create(animation);
    auto sequence = Sequence::create(animate, CallFunc::create([this]() { defaultStateAnimChar(); }), nullptr);
    modelCharac->runAction(sequence);
}

bool MainMenu::onTouchEnded(Touch* touch, Event* unused_event)
{
    // Handle touch end if needed
    return true;
}

bool MainMenu::onTouchMoved(Touch* touch, Event* unused_event)
{
    // Handle touch move if needed
    return true;
}

void MainMenu::defaultStateAnimChar()
{
    animation = createAnimation("Idle", 10, 0.035f);
    auto animate = Animate::create(animation);
    modelCharac->runAction(animate);
}

Animation* MainMenu::createAnimation(const std::string& prefixName, int frameCount, float delay)
{
    Vector<SpriteFrame*> animFrames;

    for (int i = 0; i < frameCount; ++i)
    {
        std::string frameName = prefixName + StringUtils::format("%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    return Animation::createWithSpriteFrames(animFrames, delay);
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    // EventCustom customEndEvent("game_scene_close_event");
    // _eventDispatcher->dispatchEvent(&customEndEvent);
}

bool MainMenu::sayHello(Touch* touch, Event* unused_event)
{
    return true;
}

bool MainMenu::OnPhysicContact(cocos2d::PhysicsContact& contact) 
{
    return true;
}
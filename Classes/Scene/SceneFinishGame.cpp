#include "SceneFinishGame.h"
#include "MainMenuScene.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "ui/CocosGUI.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filereadstream.h"
#include <cstdio>

USING_NS_CC;

Scene* SceneFinishGame::createScene() {
    return SceneFinishGame::create();
}

bool SceneFinishGame::init() {
    if (!Scene::init()) {
        return false;
    }

    // Load credits from JSON file
    loadCredits("json/credits_endGame.json");
    return true;
}

void SceneFinishGame::loadCredits(const std::string& jsonFilePath) {
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(jsonFilePath);
    FILE* fp = fopen(filePath.c_str(), "rb");
    if (!fp) {
        CCLOG("Failed to open JSON file: %s", filePath.c_str());
        return;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document document;
    document.ParseStream(is);
    fclose(fp);

    if (document.HasParseError()) {
        CCLOG("Failed to parse JSON file: %s", filePath.c_str());
        return;
    }

    createScrollingCredits(document);
}

void SceneFinishGame::createScrollingCredits(const rapidjson::Document& document) {
    if (!document.IsObject()) return;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float startY = -visibleSize.height / 10; // Start from below the screen
    float timeRun = 40.0f;

    for (auto& member : document.GetObject()) {
        std::string role = member.name.GetString();
        std::string name = member.value.GetString();

        // Create label for role
        auto roleLabel = Label::createWithTTF(role, Constants::FONT_ROLE_CREDIT, 19);
        roleLabel->setTextColor(Color4B::WHITE);
        roleLabel->setPosition(Vec2(visibleSize.width / 2, startY));
        this->addChild(roleLabel);

        // Create label for name
        auto nameLabel = Label::createWithTTF(name, Constants::FONT_NAME_CREDIT, 26);
        nameLabel->setTextColor(Color4B::YELLOW);
        nameLabel->setPosition(Vec2(visibleSize.width / 2, startY - roleLabel->getContentSize().height - SpriteController::calculateScreenRatio(0.007f)));
        this->addChild(nameLabel);

        // Create a move action to scroll the credits from bottom to top
        auto moveActionRole = MoveBy::create(timeRun, Vec2(0, visibleSize.height * 4 + roleLabel->getContentSize().height));
        roleLabel->runAction(moveActionRole);

        auto moveActionName = MoveBy::create(timeRun, Vec2(0, visibleSize.height * 4 + nameLabel->getContentSize().height - 13));
        nameLabel->runAction(moveActionName);

        startY -= (roleLabel->getContentSize().height + nameLabel->getContentSize().height + SpriteController::calculateScreenRatio(0.04f)); // Adjust spacing between credits
    }

    auto delay = DelayTime::create(timeRun + 1.5);
    auto callback = CallFunc::create([]() {
        auto mainMenuScene = MainMenu::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, mainMenuScene));
        });
    auto sequence = Sequence::create(delay, callback, nullptr);

    SoundController::getInstance()->playMusic(Constants::SoundRadioEndGame, false);

    this->runAction(sequence);
}

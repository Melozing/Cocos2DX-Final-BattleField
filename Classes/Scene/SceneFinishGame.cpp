#include "SceneFinishGame.h"
#include "Constants/Constants.h"
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
    float startY = visibleSize.height;

    for (auto& member : document.GetObject()) {
        std::string role = member.name.GetString();
        std::string name = member.value.GetString();

        std::string creditText = role + ": " + name;
        auto label = Label::createWithTTF(creditText, Constants::FONT_GAME, 24);
        label->setPosition(Vec2(visibleSize.width / 2, startY));
        this->addChild(label);

        // Create a move action to scroll the credits
        auto moveAction = MoveBy::create(10.0f, Vec2(0, -visibleSize.height - label->getContentSize().height));
        label->runAction(moveAction);

        startY += label->getContentSize().height + 20; // Adjust spacing between credits
    }
}

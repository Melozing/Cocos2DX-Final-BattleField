// EffectObject.h
#ifndef EFFECT_OBJECT_H
#define EFFECT_OBJECT_H

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class EffectObject : public cocos2d::Sprite, public SpriteController {
public:
    static EffectObject* create(const std::string& plistFile, const std::string& textureFile);

    // Constructor and Destructor
    EffectObject();
    virtual ~EffectObject();
    bool init() override;
    void reset();

    // Animation handling
    void playAnimation(const std::string& namePrefix, int frameCount, float delay);
    void stopAnimation();

    // Setters for plist and texture files
    void setPlistFile(const std::string& plistFile);
    void setTextureFile(const std::string& textureFile);

private:
    cocos2d::SpriteBatchNode* _spriteBatchNode;
    cocos2d::Sprite* _effectSprite;

    std::string _plistFile;
    std::string _textureFile;
};

#endif // EFFECT_OBJECT_H

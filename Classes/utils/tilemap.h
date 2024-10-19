#pragma once
#include "cocos2d.h"

namespace MyGameNamespace {

    struct Tile {
        bool is_blocking;
        cocos2d::Vec2 position;
        cocos2d::Size size;
        cocos2d::Sprite* sprite;
    };

    class Tilemap : public cocos2d::Node {
    public:
        Tilemap();
        ~Tilemap();
        void initTilemap(const cocos2d::Vec2& pos);
        void update(float deltaTime);
        void renderTilemap(cocos2d::DrawNode* drawNode);
        bool isFreeTile(float x, float y) const;
        bool isFreeTile(const cocos2d::Vec2& pos) const;

    private:
        void clearObstacles();
        void drawTile(cocos2d::DrawNode* drawNode, Tile& tile);

        cocos2d::Vec2 pos;
        std::vector<Tile> tiles;
    };

} // namespace MyGameNamespace

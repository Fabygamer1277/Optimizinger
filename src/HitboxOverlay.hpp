#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class HitboxOverlay : public CCNode {
protected:
    CCDrawNode* m_draw = nullptr;

    bool init() override {
        if (!CCNode::init())
            return false;

        m_draw = CCDrawNode::create();
        this->addChild(m_draw);

        this->scheduleUpdate();

        return true;
    }

    void drawPlayerHitbox(PlayerObject* player) {
        if (!player || !player->isVisible())
            return;

        auto pos = player->getPosition();

        constexpr float width = 30.f;
        constexpr float height = 30.f;

        CCPoint points[4] = {
            { pos.x - width / 2,  pos.y - height / 2 },
            { pos.x + width / 2,  pos.y - height / 2 },
            { pos.x + width / 2,  pos.y + height / 2 },
            { pos.x - width / 2,  pos.y + height / 2 }
        };

        m_draw->drawPolygon(
            points,
            4,
            ccc4f(0.f, 1.f, 0.f, 0.15f),
            2.f,
            ccc4f(0.f, 1.f, 0.f, 1.f)
        );
    }

    void drawObjectHitbox(GameObject* obj) {
        if (!obj || !obj->isVisible())
            return;

        auto rect = obj->getObjectRect();

        CCPoint points[4] = {
            { rect.getMinX(), rect.getMinY() },
            { rect.getMaxX(), rect.getMinY() },
            { rect.getMaxX(), rect.getMaxY() },
            { rect.getMinX(), rect.getMaxY() }
        };

        m_draw->drawPolygon(
            points,
            4,
            ccc4f(1.f, 0.f, 0.f, 0.10f),
            1.5f,
            ccc4f(1.f, 0.f, 0.f, 0.9f)
        );
    }

    void update(float) override {
        m_draw->clear();

        auto playLayer = PlayLayer::get();
        if (!playLayer)
            return;

        drawPlayerHitbox(playLayer->m_player1);
        drawPlayerHitbox(playLayer->m_player2);

        auto objects = playLayer->m_objects;
        if (!objects)
            return;

        CCObject* obj;
        CCARRAY_FOREACH(objects, obj) {
            auto gameObj = static_cast<GameObject*>(obj);
            drawObjectHitbox(gameObj);
        }
    }

public:
    static HitboxOverlay* create() {
        auto ret = new HitboxOverlay();

        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

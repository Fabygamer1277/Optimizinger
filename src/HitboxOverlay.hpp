#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Sahnedeki tüm node'ların hitbox'ını çizen overlay
class HitboxOverlay : public CCNode {
protected:
    CCDrawNode* m_draw;

    bool init() {
        if (!CCNode::init()) return false;

        m_draw = CCDrawNode::create();
        this->addChild(m_draw);

        // Her frame güncelle
        this->scheduleUpdate();

        return true;
    }

    // Bir node ve tüm çocuklarını recursive çiz
    void drawNode(CCNode* node) {
        if (!node || !node->isVisible()) return;

        auto bb = node->boundingBox();
        auto parent = node->getParent();

        // Koordinatları dünya uzayına çevir
        CCPoint worldOrigin = parent
            ? parent->convertToWorldSpace(bb.origin)
            : bb.origin;

        float w = bb.size.width  * node->getScaleX();
        float h = bb.size.height * node->getScaleY();

        // Renk: dokunabilir (CCMenuItem) → yeşil, diğerleri → kırmızı
        bool isTouchable = dynamic_cast<CCMenuItem*>(node) != nullptr;
        ccColor4F color = isTouchable
            ? ccc4f(0.1f, 1.0f, 0.1f, 0.35f)
            : ccc4f(1.0f, 0.1f, 0.1f, 0.20f);
        ccColor4F border = isTouchable
            ? ccc4f(0.0f, 1.0f, 0.0f, 0.9f)
            : ccc4f(1.0f, 0.3f, 0.3f, 0.7f);

        // Dünya koordinatına çevrilmiş köşeler
        CCPoint pts[4] = {
            worldOrigin,
            { worldOrigin.x + w, worldOrigin.y },
            { worldOrigin.x + w, worldOrigin.y + h },
            { worldOrigin.x,     worldOrigin.y + h },
        };

        m_draw->drawPolygon(pts, 4, color, 1.0f, border);

        // Çocukları da çiz
        auto children = node->getChildren();
        if (!children) return;
        for (unsigned i = 0; i < children->count(); i++) {
            drawNode(static_cast<CCNode*>(children->objectAtIndex(i)));
        }
    }

    void update(float dt) override {
        m_draw->clear();

        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (!scene) return;

        auto children = scene->getChildren();
        if (!children) return;

        for (unsigned i = 0; i < children->count(); i++) {
            auto child = static_cast<CCNode*>(children->objectAtIndex(i));
            // Overlay kendini çizmesin
            if (child == this) continue;
            drawNode(child);
        }
    }

public:
    static HitboxOverlay* create() {
        auto ret = new HitboxOverlay();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

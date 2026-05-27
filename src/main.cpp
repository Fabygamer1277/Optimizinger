#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "EmirGui.hpp"
#include "HitboxOverlay.hpp"

using namespace geode::prelude;

// Overlay'in sahnede var olup olmadığını tutan global tag
static constexpr int HITBOX_OVERLAY_TAG = 9341;

// Overlay'i mevcut sahneden alır (yoksa nullptr)
static HitboxOverlay* getOverlay() {
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene) return nullptr;
    return static_cast<HitboxOverlay*>(scene->getChildByTag(HITBOX_OVERLAY_TAG));
}

// Overlay'i aç / kapat
static void setHitboxVisible(bool visible) {
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene) return;

    if (visible) {
        if (!getOverlay()) {
            auto overlay = HitboxOverlay::create();
            overlay->setTag(HITBOX_OVERLAY_TAG);
            // En üste çıksın
            overlay->setZOrder(9999);
            scene->addChild(overlay);
        }
    } else {
        if (auto overlay = getOverlay()) {
            overlay->removeFromParent();
        }
    }
}

class $modify(MyMenuLayer, MenuLayer) {
    // Kendi alanlarımızı struct içinde tutuyoruz
    struct Fields {
        bool hitboxActive = false;
    };

    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("bottom-menu");
        if (!menu) return true; // güvenlik

        // Toggle butonu oluştur
        auto toggle = EmirGui::createToggleBtn(
            "HB ON",   // açıkken görünen yazı
            "HB",      // kapalıyken görünen yazı
            this,
            menu_selector(MyMenuLayer::onHitboxToggle)
        );
        toggle->setID("hitbox-toggle"_spr);

        menu->addChild(toggle);
        menu->updateLayout();

        return true;
    }

    void onHitboxToggle(CCObject* sender) {
        m_fields->hitboxActive = !m_fields->hitboxActive;
        setHitboxVisible(m_fields->hitboxActive);

        // Kullanıcıya kısa bildirim
        Notification::create(
            m_fields->hitboxActive ? "Hitbox'lar açıldı" : "Hitbox'lar kapatıldı",
            NotificationIcon::None
        )->show();
    }
};

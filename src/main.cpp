#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/ui/Notification.hpp>

#include "EmirGui.hpp"
#include "HitboxOverlay.hpp"

using namespace geode::prelude;

static constexpr int HITBOX_OVERLAY_TAG = 9341;
static bool g_hitboxEnabled = true;

static HitboxOverlay* getOverlay() {
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene) return nullptr;

    return static_cast<HitboxOverlay*>(
        scene->getChildByTag(HITBOX_OVERLAY_TAG)
    );
}

static void createOverlay() {
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene || getOverlay()) return;

    auto overlay = HitboxOverlay::create();
    overlay->setTag(HITBOX_OVERLAY_TAG);
    overlay->setZOrder(999999);

    scene->addChild(overlay);
}

static void removeOverlay() {
    if (auto overlay = getOverlay()) {
        overlay->removeFromParentAndCleanup(true);
    }
}

static void updateOverlayState() {
    if (g_hitboxEnabled)
        createOverlay();
    else
        removeOverlay();
}

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        updateOverlayState();
        return true;
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields {
        CCMenu* guiMenu = nullptr;
        CCMenuItemToggler* toggle = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        createGUI();
        updateOverlayState();

        return true;
    }

    void createGUI() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_fields->guiMenu = CCMenu::create();
        m_fields->guiMenu->setPosition({ 120.f, winSize.height - 80.f });
        m_fields->guiMenu->setZOrder(999999);

        this->addChild(m_fields->guiMenu);

        auto bg = CCScale9Sprite::create("square02_small.png");
        bg->setContentSize({ 140.f, 70.f });
        bg->setOpacity(120);
        bg->setColor({ 0, 0, 0 });
        bg->setPosition({ 0.f, 0.f });

        m_fields->guiMenu->addChild(bg, -1);

        auto title = CCLabelBMFont::create("HITBOX GUI", "goldFont.fnt");
        title->setScale(0.45f);
        title->setPosition({ 0.f, 22.f });

        m_fields->guiMenu->addChild(title);

        m_fields->toggle = EmirGui::createToggleBtn(
            "ON",
            "OFF",
            this,
            menu_selector(MyPlayLayer::onToggle)
        );

        m_fields->toggle->toggle(g_hitboxEnabled);
        m_fields->toggle->setPosition({ 0.f, -10.f });

        m_fields->guiMenu->addChild(m_fields->toggle);

        this->setTouchEnabled(true);
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        auto pos = m_fields->guiMenu->convertToNodeSpace(touch->getLocation());

        CCRect rect = CCRect(-70.f, -35.f, 140.f, 70.f);

        return rect.containsPoint(pos);
    }

    void ccTouchMoved(CCTouch* touch, CCEvent* event) {
        auto delta = touch->getDelta();

        auto current = m_fields->guiMenu->getPosition();
        m_fields->guiMenu->setPosition({
            current.x + delta.x,
            current.y + delta.y
        });
    }

    void onToggle(CCObject*) {
        g_hitboxEnabled = !g_hitboxEnabled;

        updateOverlayState();

        Notification::create(
            g_hitboxEnabled
                ? "Real hitbox overlay enabled"
                : "Real hitbox overlay disabled",
            NotificationIcon::Success
        )->show();
    }
};

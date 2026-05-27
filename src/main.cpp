#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "EmirGui.hpp"

using namespace geode::prelude;

// Global durum değişkenleri
bool g_showHitboxes = false;
EmirGui::DraggablePanel* g_panel = nullptr;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontRunActions) {
        if (!PlayLayer::init(level, useReplay, dontRunActions)) return false;
        if (g_showHitboxes) this->m_showHitbox = true;
        return true;
    }
};

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // Panel oluşturma
        if (!g_panel) {
            g_panel = EmirGui::DraggablePanel::create(200, 150);
            g_panel->setPosition({100, 100});
            g_panel->setVisible(false);
            this->addChild(g_panel, 100);

            auto menu = CCMenu::create();
            menu->setPosition({100, 75});
            g_panel->addChild(menu);

            auto label = CCLabelBMFont::create("Hitbox", "bigFont.fnt");
            label->setScale(0.4f);
            label->setPosition({0, 20});
            menu->addChild(label);

            auto toggle = CCMenuItemToggler::createWithStandardSprites(
                this, menu_selector(MyMenuLayer::onToggle), 0.8f
            );
            toggle->toggle(g_showHitboxes);
            menu->addChild(toggle);
        }

        // Açma/Kapatma Butonu
        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
            this, menu_selector(MyMenuLayer::onOpenPanel)
        );
        auto menu = this->getChildByID("bottom-menu");
        menu->addChild(btn);
        menu->updateLayout();

        return true;
    }

    void onOpenPanel(CCObject*) {
        if (g_panel) g_panel->setVisible(!g_panel->isVisible());
    }

    void onToggle(CCObject* sender) {
        g_showHitboxes = !g_showHitboxes;
    }
};

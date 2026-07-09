#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

class $modify(MenuLayer) {
    bool init() override {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            auto sprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
            auto btn = CCMenuItemSpriteExtra::create(
                sprite, this, menu_selector(MenuLayer::onMyOptimizationMenu)
            );
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onMyOptimizationMenu(CCObject* sender) {
        auto layer = MyOptimizationMenu::create("Datos de optimización");
        if (layer) {
            layer->show();
        }
    }
};

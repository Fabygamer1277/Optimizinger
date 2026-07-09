#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

// Usamos dos argumentos para poder agregar funciones personalizadas de forma segura
class $modify(MyMenuLayer, MenuLayer) {
    bool init() override {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            auto sprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
            
            auto btn = CCMenuItemSpriteExtra::create(
                sprite, 
                this, 
                menu_selector(MyMenuLayer::onMyOptimizationMenu) // CORREGIDO: Apunta a MyMenuLayer
            );
            
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    // Esta función ahora pertenece a nuestro contenedor del hook y es segura de enlazar
    void onMyOptimizationMenu(cocos2d::CCObject* sender) {
        auto layer = MyOptimizationMenu::create("Datos de optimización");
        if (layer) {
            layer->show();
        }
    }
};

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (bottomMenu) {
            // Cargamos la imagen verde redonda renombrada como buttom_open.png
            auto mySprite = CCSprite::create("buttom_open.png");
            
            if (mySprite) {
                mySprite->setScale(0.35f); 

                auto myButton = CCMenuItemSpriteExtra::create(
                    mySprite,
                    this,
                    menu_selector(MenuLayerHook::onOpenOptimizationMenu)
                );

                bottomMenu->addChild(myButton);
                bottomMenu->updateLayout();
            }
        }

        return true;
    }

    void onOpenOptimizationMenu(cocos2d::CCObject* sender) {
        auto menuLayer = MyOptimizationMenu::create();
        if (menuLayer) {
            menuLayer->show(); // Dispara tu ventana flotante personalizada
        }
    }
};

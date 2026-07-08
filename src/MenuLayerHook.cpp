#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    // 1. Ponemos la función aquí arriba para que init() sepa que existe
    void onOpenOptimizationMenu(cocos2d::CCObject* sender) {
        auto menuLayer = MyOptimizationMenu::create();
        if (menuLayer) {
            menuLayer->show();
        }
    }

    bool init() {
        if (!MenuLayer::init()) return false;

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (bottomMenu) {
            auto mySprite = CCSprite::create("buttom_open.png");
            
            if (mySprite) {
                mySprite->setScale(0.35f); 

                auto myButton = CCMenuItemSpriteExtra::create(
                    mySprite,
                    this,
                    menu_selector(MyMenuLayer::onOpenOptimizationMenu) // Usamos el nombre de nuestro contenedor
                );

                bottomMenu->addChild(myButton);
                bottomMenu->updateLayout();
            }
        }

        return true;
    }
};

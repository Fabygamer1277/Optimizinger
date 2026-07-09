#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

static float g_targetTPS = 60.0f;
static float g_targetFPS = 60.0f;
static bool  g_syncEnabled = false;

// =================================================================
// HOOK DE LA FÍSICA DEL JUEGO (PlayLayer)
// =================================================================
class $modify(PlayLayer) {
    void update(float dt) override {
        int savedTPS = Mod::get()->getSavedValue<int>("tps", 60);
        int savedFPS = Mod::get()->getSavedValue<int>("fps", 60);
        
        float tps = g_syncEnabled ? (float)savedFPS : (float)savedTPS;
        PlayLayer::update(1.0f / tps);
    }
};

// =================================================================
// HOOK DE LA PANTALLA DE PAUSA (PauseLayer)
// =================================================================
class $modify(MyPauseLayer, PauseLayer) { // Usamos dos argumentos para poder declarar métodos nuevos de forma segura enlazados
    
    void onMyMenuButton(cocos2d::CCObject* sender) { 
        auto layer = MyOptimizationMenu::create();
        if (layer) {
            layer->show(); 
        }
    }

    void customSetup() {
        PauseLayer::customSetup();
        
        g_targetTPS = (float)Mod::get()->getSavedValue<int>("tps", 60);
        g_targetFPS = (float)Mod::get()->getSavedValue<int>("fps", 60);

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        this->addChild(menu, 100);

        auto spr = CCSprite::create("buttom_open.png"_spr);
        if (spr) {
            spr->setScale(0.28f); 

            auto btn = CCMenuItemSpriteExtra::create(
                spr, 
                this, 
                menu_selector(MyPauseLayer::onMyMenuButton) // Apunta a MyPauseLayer
            );
            
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            btn->setPosition({35.0f, winSize.height - 75.0f});
            
            menu->addChild(btn);
        }
    }
};

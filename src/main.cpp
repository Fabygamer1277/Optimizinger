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
class $modify(MyPauseLayer, PauseLayer) { // Usamos dos argumentos para declarar métodos nuevos de forma segura
    
    void onMyMenuButton(cocos2d::CCObject* sender) { 
        // Obtenemos el valor guardado para mostrárselo al usuario en el menú
        int savedTPS = Mod::get()->getSavedValue<int>("tps", 60);
        std::string tpsTexto = std::to_string(savedTPS) + " TPS";

        // CORREGIDO: Ahora le pasamos el string requerido a la función create
        auto layer = MyOptimizationMenu::create(tpsTexto);
        if (layer) {
            layer->show(); 
        }
    }

    void customSetup() override { // Añadido override por buena práctica de Geode
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

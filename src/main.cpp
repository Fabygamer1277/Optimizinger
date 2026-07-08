#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "MyOptimizationMenu.hpp" // Enlazamos con tu archivo separado

using namespace geode::prelude;

// Mantener las variables globales y de persistencia aquí para que afecten al juego directamente
static float g_targetTPS = 60.0f;
static float g_targetFPS = 60.0f;
static bool  g_syncEnabled = false;

// =================================================================
// HOOK DE LA FÍSICA DEL JUEGO (PlayLayer)
// =================================================================
class $modify(PlayLayer) {
    void update(float dt) override {
        // Leemos los valores guardados en tiempo real por Geode al actualizar cada frame
        int savedTPS = Mod::get()->getSavedValue<int>("tps", 60);
        int savedFPS = Mod::get()->getSavedValue<int>("fps", 60);
        
        float tps = g_syncEnabled ? (float)savedFPS : (float)savedTPS;
        PlayLayer::update(1.0f / tps);
    }
};

// =================================================================
// HOOK DE LA PANTALLA DE PAUSA (PauseLayer)
// =================================================================
class $modify(PauseLayer) {
    
    // Movido arriba de customSetup para que el compilador sepa que existe antes de ser llamado
    void onMyMenuButton(CCObject* sender) { 
        // Llamamos al método create() del menú que movimos a MyOptimizationMenu.cpp
        auto layer = MyOptimizationMenu::create();
        if (layer) {
            layer->show(); 
        }
    }

    void customSetup() {
        PauseLayer::customSetup();
        
        // Sincronizamos los valores iniciales guardados al pausar
        g_targetTPS = (float)Mod::get()->getSavedValue<int>("tps", 60);
        g_targetFPS = (float)Mod::get()->getSavedValue<int>("fps", 60);

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        this->addChild(menu, 100);

        // Cargamos tu textura redonda verde
        auto spr = CCSprite::create("buttom_open.png"_spr);
        if (spr) {
            spr->setScale(0.28f); 

            auto btn = CCMenuItemSpriteExtra::create(
                spr, 
                this, 
                menu_selector(PauseLayer::onMyMenuButton) // CORREGIDO: Apunta directamente a PauseLayer
            );
            
            // Posicionamiento en la esquina superior izquierda
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            btn->setPosition({35.0f, winSize.height - 75.0f});
            
            menu->addChild(btn);
        }
    }
};

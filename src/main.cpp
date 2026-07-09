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
        // Obtenemos los valores de forma segura evitando llamadas pesadas en el bucle principal
        auto myMod = Mod::get();
        if (!myMod) {
            PlayLayer::update(dt);
            return;
        }

        int savedTPS = myMod->getSavedValue<int>("tps", 60);
        int savedFPS = myMod->getSavedValue<int>("fps", 60);
        
        // Evitamos división por cero de forma estricta como dice el manual
        float tps = g_syncEnabled ? (float)savedFPS : (float)savedTPS;
        if (tps <= 0.f) tps = 60.0f;

        // Escalamos el delta original manteniendo la física sincronizada con la velocidad del nivel
        float juegoVelocidadFactor = dt * 60.0f; 
        PlayLayer::update(juegoVelocidadFactor / tps);
    }
};

// =================================================================
// HOOK DE LA PANTALLA DE PAUSA (PauseLayer)
// =================================================================
class $modify(MyPauseLayer, PauseLayer) {
    
    void onMyMenuButton(cocos2d::CCObject* sender) { 
        auto myMod = Mod::get();
        if (!myMod) return;

        int savedTPS = myMod->getSavedValue<int>("tps", 60);
        std::string tpsTexto = std::to_string(savedTPS) + " TPS";

        auto layer = MyOptimizationMenu::create(tpsTexto);
        if (layer) {
            layer->show(); 
        }
    }

    void customSetup() override {
        PauseLayer::customSetup();
        
        auto myMod = Mod::get();
        if (myMod) {
            g_targetTPS = (float)myMod->getSavedValue<int>("tps", 60);
            g_targetFPS = (float)myMod->getSavedValue<int>("fps", 60);
        }

        auto menu = CCMenu::create();
        if (!menu) return; // Control de punteros nulos de Cocos
        
        menu->setPosition({0, 0});
        this->addChild(menu, 100);

        // Usamos un sprite nativo del juego para asegurar que cargue bien en cualquier cel
        auto spr = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        if (spr) {
            spr->setScale(0.7f); 

            auto btn = CCMenuItemSpriteExtra::create(
                spr, 
                this, 
                menu_selector(MyPauseLayer::onMyMenuButton)
            );
            
            if (btn) {
                auto winSize = CCDirector::sharedDirector()->getWinSize();
                btn->setPosition({35.0f, winSize.height - 75.0f});
                menu->addChild(btn);
            }
        }
    }
};

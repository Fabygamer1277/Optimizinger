#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Valor de TPS global
static float g_targetTPS = 60.0f; 

// ============================================================================
// 1. UI MENU DEFINITION
// ============================================================================
class MyOptimizationMenu : public FLAlertLayer {
protected:
    InputNode* m_input;

    bool init() override {
        if (!FLAlertLayer::init(150)) return false;
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({320, 240});
        bg->setPosition(winSize / 2);
        this->addChild(bg);

        // Input restringido a números enteros
        m_input = InputNode::create(100, "TPS (ej: 144)");
        m_input->setPosition(winSize / 2);
        // Filtrar decimales: GD InputNode a veces permite todo, 
        // aquí forzamos la lectura como entero al guardar.
        m_input->setString(std::to_string((int)Mod::get()->getSavedValue<int>("tps-val", 60)));
        this->addChild(m_input);

        auto saveBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Save"), this, menu_selector(MyOptimizationMenu::onSave));
        auto menu = CCMenu::create();
        menu->addChild(saveBtn);
        menu->setPosition({winSize.width / 2, (winSize.height / 2) - 50});
        this->addChild(menu);

        return true;
    }

    void onSave(CCObject*) {
        std::string raw = m_input->getString();
        // Limpieza: quitamos cualquier punto si el usuario lo pone
        size_t point = raw.find('.');
        if (point != std::string::npos) raw = raw.substr(0, point);
        
        int val = std::stoi(raw);
        if (val > 1 && val < 9999) {
            Mod::get()->setSavedValue<int>("tps-val", val);
            g_targetTPS = (float)val;
            // Cálculo dinámico de ms
            float ms = 1000.0f / val;
            Notification::create(fmt::format("TPS: {} | {:.2f} ms por tick", val, ms))->show();
        }
    }
    
    void onClose(CCObject* s) { this->removeFromParentAndCleanup(true); }
};

// ============================================================================
// 2. MOTOR FÍSICO DINÁMICO
// ============================================================================
class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        // El motor ahora procesa la física usando tu TPS exacto
        PlayLayer::update(1.0f / g_targetTPS);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void onMyMenuButton(CCObject*) { MyOptimizationMenu::create()->show(); }

    void customSetup() {
        PauseLayer::customSetup();
        
        // Cargar valor persistente
        g_targetTPS = (float)Mod::get()->getSavedValue<int>("tps-val", 60);

        auto myMenu = CCMenu::create();
        myMenu->setPosition({45.0f, CCDirector::sharedDirector()->getWinSize().height - 75.0f});
        this->addChild(myMenu, 100);

        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"), 
            this, menu_selector(MyPauseLayer::onMyMenuButton));
        myMenu->addChild(btn);
    }
};

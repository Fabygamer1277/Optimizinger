#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

// Variable global para controlar los TPS
static float g_targetTPS = 60.0f;

// ============================================================================
// CLASE DEL MENU (POPUP)
// ============================================================================
class MyOptimizationMenu : public Popup {
protected:
    TextInput* m_input;

    bool setup() override {
        // En un Popup de Geode, usamos el mainLayer para añadir hijos manualmente
        // o nos aseguramos de que el template esté bien configurado.
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        this->setTitle("Configuracion de TPS");

        m_input = TextInput::create(100, "TPS");
        m_input->setPosition(winSize / 2);
        m_input->setString(std::to_string((int)Mod::get()->getSavedValue<int>("tps-val", 60)));
        this->m_mainLayer->addChild(m_input);

        auto saveBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Guardar"), this, menu_selector(MyOptimizationMenu::onSave));
        saveBtn->setPosition({0, -60});
        
        auto menu = CCMenu::create();
        menu->addChild(saveBtn);
        this->m_mainLayer->addChild(menu);

        return true;
    }

    void onSave(CCObject*) {
        std::string raw = m_input->getString();
        try {
            int val = std::stoi(raw.empty() ? "60" : raw);
            if (val > 1 && val < 9999) {
                Mod::get()->setSavedValue<int>("tps-val", val);
                g_targetTPS = (float)val;
                Notification::create("TPS fijado a: " + std::to_string(val))->show();
            }
        } catch (...) {
            Notification::create("Valor invalido")->show();
        }
    }

public:
    static MyOptimizationMenu* create() {
        auto ret = new MyOptimizationMenu();
        if (ret && ret->initAnchored(320.0f, 240.0f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ============================================================================
// LÓGICA DE JUEGO (HOOKS)
// ============================================================================
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        PlayLayer::update(1.0f / g_targetTPS);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void onMyMenuButton(CCObject*) { 
        MyOptimizationMenu::create()->show(); 
    }

    void customSetup() {
        PauseLayer::customSetup();
        
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

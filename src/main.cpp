#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

static float g_targetTPS = 60.0f;

// Cambiamos a 'public Popup' sin los <> para ver si el compilador lo acepta mejor
class MyOptimizationMenu : public Popup {
protected:
    TextInput* m_input;

    // Cambiamos setup() para que no sea override si el compilador sigue dando problemas,
    // pero intentaremos con override primero.
    bool setup() override {
        this->setTitle("Configuracion de TPS");

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_input = TextInput::create(100, "TPS");
        m_input->setPosition(winSize / 2);
        
        // Usamos m_mainLayer->addChild directamente, es más seguro en 5.7+
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
            Mod::get()->setSavedValue<int>("tps-val", val);
            g_targetTPS = (float)val;
            Notification::create("Guardado")->show();
        } catch (...) {
            Notification::create("Error")->show();
        }
    }

public:
    static MyOptimizationMenu* create() {
        auto ret = new MyOptimizationMenu();
        // Usamos init de Popup básico
        if (ret && ret->init(320.0f, 240.0f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

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

        auto menu = CCMenu::create();
        menu->setPosition({45.0f, CCDirector::sharedDirector()->getWinSize().height - 75.0f});
        this->addChild(menu, 100);

        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"), 
            this, menu_selector(MyPauseLayer::onMyMenuButton));
        menu->addChild(btn);
    }
};

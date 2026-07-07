#include <Geode/Geode.hpp>

using namespace geode::prelude;

static float g_targetTPS = 60.0f;

class MyOptimizationMenu : public FLAlertLayer {
protected:
    TextInput* m_input;

    // 1. Firma correcta del init con 9 argumentos
    bool init() override {
        // Añadido el 1.0f final (textScale)
        if (!FLAlertLayer::init(nullptr, "Configuracion de TPS", "", "Guardar", nullptr, 320.0f, false, 200.0f, 1.0f)) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        m_input = TextInput::create(100, "TPS");
        m_input->setPosition(winSize / 2);
        m_mainLayer->addChild(m_input);

        return true;
    }

    // 2. Quitamos 'override' porque FLAlertLayer no tiene onBtn1 virtual
    void onBtn1(CCObject* sender) {
        std::string raw = m_input->getString();
        try {
            int val = std::stoi(raw.empty() ? "60" : raw);
            Mod::get()->setSavedValue<int>("tps-val", val);
            g_targetTPS = (float)val;
            Notification::create("Guardado")->show();
            
            // 3. Forma correcta de cerrar: quitar de la pantalla
            this->setKeypadEnabled(false);
            this->removeFromParent();
        } catch (...) {
            Notification::create("Error: Numero invalido")->show();
        }
    }

public:
    static MyOptimizationMenu* create() {
        auto ret = new MyOptimizationMenu();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// El resto de tu código (Hooks de PauseLayer/PlayLayer) se mantiene igual
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

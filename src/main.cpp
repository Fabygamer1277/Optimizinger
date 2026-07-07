#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variable global para controlar el TPS
static float g_targetTPS = 60.0f;

class MyOptimizationMenu : public FLAlertLayer {
protected:
    TextInput* m_input;

    bool init() override {
        // Usamos una base de FLAlertLayer, pero personalizada
        if (!FLAlertLayer::init(nullptr, "Configuracion de TPS", "", "Guardar", nullptr, 320.0f, false, 200.0f, 1.0f)) {
            return false;
        }

        // Crear el fondo café (GJ_square01 es el estándar en GD)
        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({300.0f, 150.0f});
        bg->setPosition({160.0f, 100.0f});
        m_mainLayer->addChild(bg, -1);

        // Crear el campo de entrada de texto
        m_input = TextInput::create(150.0f, "TPS");
        m_input->setPosition({160.0f, 100.0f});
        m_input->setAllowedChars("0123456789"); // Importante: solo números
        m_input->setString(std::to_string((int)g_targetTPS)); // Pone el valor actual
        m_mainLayer->addChild(m_input);

        return true;
    }

    // El botón de guardar (onBtn1)
    void onBtn1(CCObject* sender) override {
        std::string raw = m_input->getString();
        if (!raw.empty()) {
            try {
                int val = std::stoi(raw);
                Mod::get()->setSavedValue<int>("tps-val", val);
                g_targetTPS = (float)val;
                Notification::create("TPS guardado: " + raw)->show();
            } catch (...) {
                Notification::create("Error de numero")->show();
            }
        }
        // Cerrar correctamente
        this->setKeypadEnabled(false);
        this->removeFromParent();
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

// Hook para modificar el TPS en el juego
class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        PlayLayer::update(1.0f / g_targetTPS);
    }
};

// Hook para poner el botón en el menú de pausa
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

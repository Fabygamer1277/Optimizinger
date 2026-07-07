#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Cambiamos de Popup a FLAlertLayer
class MyOptimizationMenu : public FLAlertLayer {
protected:
    TextInput* m_input;

    // FLAlertLayer usa init()
    bool init() override {
        if (!FLAlertLayer::init(nullptr, "Configuracion de TPS", "OK", nullptr, 320.0f)) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Creamos el input
        m_input = TextInput::create(100, "TPS");
        m_input->setPosition(winSize / 2);
        m_mainLayer->addChild(m_input);

        // Boton de guardar
        auto saveBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Guardar"), this, menu_selector(MyOptimizationMenu::onSave));
        saveBtn->setPosition({0, -60});
        
        auto menu = CCMenu::create();
        menu->addChild(saveBtn);
        m_mainLayer->addChild(menu);

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
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

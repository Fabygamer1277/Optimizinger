#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

static float g_targetTPS = 60.0f;

class MyOptimizationMenu : public FLAlertLayer {
protected:
    TextInput* m_input;

    bool init() override {
        // Usamos la firma correcta de 9 argumentos para FLAlertLayer
        if (!FLAlertLayer::init(nullptr, "Configuracion de TPS", "", "Guardar", nullptr, 320.0f, false, 200.0f, 1.0f)) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Fondo café clásico
        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({300.0f, 150.0f});
        bg->setPosition({winSize.width/2, winSize.height/2});
        m_mainLayer->addChild(bg, -1);

        // TextInput limpio
        m_input = TextInput::create(150.0f, "TPS");
        m_input->setPosition({winSize.width/2, winSize.height/2});
        m_input->setString(std::to_string((int)g_targetTPS));
        m_mainLayer->addChild(m_input);

        return true;
    }

    // Eliminamos 'override' porque FLAlertLayer no tiene este método como virtual
    void onBtn1(CCObject* sender) {
        std::string raw = m_input->getString();
        try {
            int val = std::stoi(raw);
            Mod::get()->setSavedValue<int>("tps-val", val);
            g_targetTPS = (float)val;
            Notification::create("TPS guardado: " + raw)->show();
        } catch (...) {
            Notification::create("Error: Numero invalido")->show();
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

#include <Geode/Geode.hpp>

using namespace geode::prelude;

static float g_targetTPS = 60.0f;

// Definimos la clase heredando de Popup directamente sin el template <>.
// Esto suele resolver el error de "unknown template name" en Android.
class MyOptimizationMenu : public Popup<> {
protected:
    TextInput* m_input;

    bool setup() override {
        this->setTitle("Configuracion de TPS");

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Usamos m_buttonMenu directamente, que es el estándar en Geode
        m_input = TextInput::create(100, "TPS");
        m_input->setPosition(winSize / 2);
        m_input->setString(std::to_string((int)Mod::get()->getSavedValue<int>("tps-val", 60)));
        this->m_buttonMenu->addChild(m_input);

        auto saveBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Guardar"), this, menu_selector(MyOptimizationMenu::onSave));
        saveBtn->setPosition({0, -60});
        this->m_buttonMenu->addChild(saveBtn);

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
        // Usamos el init estándar de 2 argumentos
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

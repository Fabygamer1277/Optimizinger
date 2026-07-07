#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables globales para los valores
static float g_targetTPS = 60.0f;
static float g_targetFPS = 60.0f;
static bool  g_cbfEnabled = false;
static bool  g_syncEnabled = false;

class MyOptimizationMenu : public FLAlertLayer {
protected:
    TextInput* m_tpsInput;
    TextInput* m_fpsInput;
    CCMenuItemToggler* m_cbfToggle;
    CCMenuItemToggler* m_syncToggle;

    bool init() override {
        if (!FLAlertLayer::init(nullptr, "Configuracion Optimizador", "", "Guardar", nullptr, 360.0f, false, 250.0f, 1.0f)) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto center = winSize / 2;

        // Fondo
        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({340.0f, 230.0f});
        bg->setPosition(center);
        m_mainLayer->addChild(bg, -1);

        // Inputs
        m_tpsInput = TextInput::create(100.0f, "TPS");
        m_tpsInput->setPosition({center.x, center.y + 60});
        m_tpsInput->setString(std::to_string((int)g_targetTPS));
        m_mainLayer->addChild(m_tpsInput);

        m_fpsInput = TextInput::create(100.0f, "FPS");
        m_fpsInput->setPosition({center.x, center.y + 20});
        m_fpsInput->setString(std::to_string((int)g_targetFPS));
        m_mainLayer->addChild(m_fpsInput);

        // Toggles (Checkboxes)
        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        m_mainLayer->addChild(menu);

        m_cbfToggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(MyOptimizationMenu::onToggle), 0.8f);
        m_cbfToggle->setPosition({center.x + 80, center.y - 20});
        m_cbfToggle->toggle(g_cbfEnabled);
        menu->addChild(m_cbfToggle);
        
        m_syncToggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(MyOptimizationMenu::onToggle), 0.8f);
        m_syncToggle->setPosition({center.x + 80, center.y - 60});
        m_syncToggle->toggle(g_syncEnabled);
        menu->addChild(m_syncToggle);

        // Etiquetas para los Toggles
        auto cbfLabel = CCLabelBMFont::create("Click Between Frames", "bigFont.fnt");
        cbfLabel->setScale(0.4f);
        cbfLabel->setPosition({center.x - 40, center.y - 20});
        m_mainLayer->addChild(cbfLabel);

        auto syncLabel = CCLabelBMFont::create("Sync TPS with FPS", "bigFont.fnt");
        syncLabel->setScale(0.4f);
        syncLabel->setPosition({center.x - 40, center.y - 60});
        m_mainLayer->addChild(syncLabel);

        return true;
    }

    void onToggle(CCObject*) {}

    void onBtn1(CCObject* sender) {
        try {
            int tps = std::stoi(m_tpsInput->getString());
            int fps = std::stoi(m_fpsInput->getString());
            
            g_targetTPS = (float)tps;
            g_targetFPS = (float)fps;
            g_cbfEnabled = m_cbfToggle->isToggled();
            g_syncEnabled = m_syncToggle->isToggled();

            Mod::get()->setSavedValue<int>("tps", tps);
            Mod::get()->setSavedValue<int>("fps", fps);
            Mod::get()->setSavedValue<bool>("cbf", g_cbfEnabled);
            Mod::get()->setSavedValue<bool>("sync", g_syncEnabled);
            
            Notification::create("Configuración guardada")->show();
        } catch (...) {
            Notification::create("Error en valores")->show();
        }
        this->setKeypadEnabled(false);
        this->removeFromParent();
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        float tps = g_syncEnabled ? g_targetFPS : g_targetTPS;
        PlayLayer::update(1.0f / tps);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        g_targetTPS = (float)Mod::get()->getSavedValue<int>("tps", 60);
        g_targetFPS = (float)Mod::get()->getSavedValue<int>("fps", 60);
        g_cbfEnabled = Mod::get()->getSavedValue<bool>("cbf", false);
        g_syncEnabled = Mod::get()->getSavedValue<bool>("sync", false);

        auto menu = CCMenu::create();
        menu->setPosition({45.0f, CCDirector::sharedDirector()->getWinSize().height - 75.0f});
        this->addChild(menu, 100);

        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"), 
            this, menu_selector(MyPauseLayer::onMyMenuButton));
        menu->addChild(btn);
    }
    void onMyMenuButton(CCObject*) { MyOptimizationMenu::create()->show(); }
};

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

static float g_targetTPS = 60.0f;

// ============================================================================
// POPUP CORRECTO (La forma moderna en Geode)
// ============================================================================
class MyOptimizationMenu : public Popup<> {
protected:
    InputNode* m_input;

    bool setup() override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        this->setTitle("FPS/TPS");

        m_input = InputNode::create(100, "TPS", "bigFont.fnt", 5);
        m_input->setPosition(winSize / 2);
        m_input->setString(std::to_string((int)Mod::get()->getSavedValue<int>("tps-val", 60)));
        this->m_mainLayer->addChild(m_input);

        auto saveBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Save"), this, menu_selector(MyOptimizationMenu::onSave));
        auto menu = CCMenu::create();
        menu->addChild(saveBtn);
        menu->setPosition({winSize.width / 2, (winSize.height / 2) - 50});
        this->m_mainLayer->addChild(menu);

        return true;
    }

    void onSave(CCObject*) {
        std::string raw = m_input->getString();
        size_t point = raw.find('.');
        if (point != std::string::npos) raw = raw.substr(0, point);
        
        int val = std::stoi(raw.empty() ? "60" : raw);
        if (val > 1 && val < 9999) {
            Mod::get()->setSavedValue<int>("tps-val", val);
            g_targetTPS = (float)val;
            Notification::create("TPS guardado: " + std::to_string(val))->show();
        }
    }

public:
    static MyOptimizationMenu* create() {
        auto ret = new MyOptimizationMenu();
        if (ret && ret->init(320, 240)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ============================================================================
// LÓGICA DE FÍSICA
// ============================================================================
class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        PlayLayer::update(1.0f / g_targetTPS);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void onMyMenuButton(CCObject*) { MyOptimizationMenu::create()->show(); }

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

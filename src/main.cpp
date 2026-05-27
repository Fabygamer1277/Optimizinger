#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// Noclip durumunu saklamak için global değişken
bool g_noclipEnabled = false;

// PlayLayer kancası: Oyun içi Noclip mantığı
class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        if (g_noclipEnabled && this->m_player1) {
            this->m_player1->m_isDead = false;
        }
        PlayLayer::update(dt);
    }
};

// GUI Katmanı: Ekranda küçük bir kutu oluşturur
class NoclipGui : public cocos2d::CCLayer {
public:
    static NoclipGui* create() {
        auto ret = new NoclipGui();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() override {
        // Bir buton oluştur (toggle görevi görecek)
        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Noclip: OFF", "bigFont.fnt", "GJ_button_01.png", .8f),
            this,
            menu_selector(NoclipGui::onToggle)
        );
        
        auto menu = CCMenu::create();
        menu->setPosition({ 100, 50 }); // Ekrandaki yeri
        menu->addChild(btn);
        this->addChild(menu);
        
        return true;
    }

    void onToggle(CCObject* sender) {
        g_noclipEnabled = !g_noclipEnabled;
        
        // Buton yazısını güncelle
        auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
        auto label = static_cast<ButtonSprite*>(btn->getChildren()->objectAtIndex(0));
        label->setString(g_noclipEnabled ? "Noclip: ON" : "Noclip: OFF");
    }
};

// Menüye GUI'yi ekle
class $modify(MenuLayer) {
    bool init() override {
        if (!MenuLayer::init()) return false;
        
        auto gui = NoclipGui::create();
        this->addChild(gui, 100);
        
        return true;
    }
};

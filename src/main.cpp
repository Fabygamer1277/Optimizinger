#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

// 1. Popup Sınıfı: Referans dosyasındaki FLAlertLayer ve Popup mantığına uygun
class EmirMenu : public geode::Popup<> {
protected:
    // setup() fonksiyonu geode::Popup'ın standartıdır
    bool setup() override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Başlık
        auto title = CCLabelBMFont::create("Emir Mod", "bigFont.fnt");
        title->setPosition({winSize.width / 2, winSize.height / 2 + 60});
        this->m_mainLayer->addChild(title);

        // Menu oluştur (Toggle'lar için taşıyıcı)
        auto menu = CCMenu::create();
        this->m_mainLayer->addChild(menu);

        // Noclip Toggle
        auto noclip = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(EmirMenu::onToggle), 0.8f);
        noclip->setPosition({0, 20});
        menu->addChild(noclip);

        // ESP Toggle
        auto esp = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(EmirMenu::onToggle), 0.8f);
        esp->setPosition({0, -20});
        menu->addChild(esp);

        return true;
    }

    void onToggle(CCObject* sender) {
        Notification::create("Ayar Değiştirildi!", NotificationIcon::Success)->show();
    }

public:
    static EmirMenu* create() {
        auto ret = new EmirMenu();
        // init(width, height)
        if (ret && ret->init(300, 200)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// 2. Ana Menü Modifikasyonu
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = static_cast<CCMenu*>(this->getChildByID("bottom-menu"));
        
        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Emir Mod"), 
            this, 
            menu_selector(MyMenuLayer::onOpenEmir)
        );
        
        btn->setID("emir-btn"_spr);
        menu->addChild(btn);
        menu->updateLayout();

        return true;
    }

    void onOpenEmir(CCObject* sender) {
        EmirMenu::create()->show();
    }
};

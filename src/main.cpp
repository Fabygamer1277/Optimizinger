#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// 1. UI MENU DEFINITION (Using proper types to fix the assigning compilation error)
// ============================================================================
class MyOptimizationMenu : public FLAlertLayer {
protected:
    bool init() {
        if (!FLAlertLayer::init(150)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Create a standard CCLayer container first to avoid type mismatch
        auto mainLayer = CCLayer::create();
        this->addChild(mainLayer);
        this->m_mainLayer = mainLayer;

        // Create and add the background texture inside our wrapper layer
        auto backgroundLayer = CCScale9Sprite::create("GJ_square01.png");
        backgroundLayer->setContentSize({320, 240});
        backgroundLayer->setPosition(winSize / 2);
        mainLayer->addChild(backgroundLayer);

        // Setup custom text title positioned relative to the center screen
        auto titleLabel = CCLabelBMFont::create("FPS/TPS", "goldFont.fnt");
        titleLabel->setPosition({winSize.width / 2, (winSize.height / 2) + 100});
        mainLayer->addChild(titleLabel);

        // Initialize user interaction menu container interface
        auto subMenu = CCMenu::create();
        subMenu->setPosition({0, 0});
        mainLayer->addChild(subMenu);

        // Standard close popup window option asset link
        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(MyOptimizationMenu::onClose)
        );
        closeButton->setPosition({(winSize.width / 2) - 145, (winSize.height / 2) + 105});
        subMenu->addChild(closeButton);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
    }

    void onClose(CCObject* sender) {
        this->removeFromParentAndCleanup(true);
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
    
    void show() {
        CCDirector::sharedDirector()->getRunningScene()->addChild(this, 100);
    }
};

// ============================================================================
// 2. PLAYLAYER HOOK (Injecting the dynamic interaction layout button)
// ============================================================================
class $modify(MyPlayLayer, PlayLayer) {
    void onMyMenuButton(CCObject* sender) {
        auto menu = MyOptimizationMenu::create();
        if (menu) {
            menu->show();
        }
    }

    bool init(GJGameLevel* level, bool useReplay, bool dontRunActions) {
        if (!PlayLayer::init(level, useReplay, dontRunActions)) return false;

        auto uiMenu = this->getChildByID("ui-menu");
        if (!uiMenu) {
            uiMenu = CCMenu::create();
            uiMenu->setPosition({20, CCDirector::sharedDirector()->getWinSize().height - 60});
            this->addChild(uiMenu);
        }

        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        
        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPlayLayer::onMyMenuButton)
        );

        myButton->setID("fps-optimizer-button");
        
        uiMenu->addChild(myButton);
        uiMenu->updateLayout();

        return true;
    }
};

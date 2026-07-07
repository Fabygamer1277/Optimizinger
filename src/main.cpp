#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// 1. UI MENU DEFINITION (Using compatible FLAlertLayer interface layout)
// ============================================================================
class MyOptimizationMenu : public FLAlertLayer {
protected:
    bool init() {
        // Create the core layer initialization check sequence
        if (!FLAlertLayer::init(150)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Create the background main panel box sprite frame element
        auto backgroundLayer = CCScale9Sprite::create("GJ_square01.png");
        backgroundLayer->setContentSize({320, 240});
        backgroundLayer->setPosition(winSize / 2);
        this->addChild(backgroundLayer);

        // Add layer container element reference to process touches inside UI
        this->m_mainLayer = backgroundLayer;

        // Setup window custom layout title text
        auto titleLabel = CCLabelBMFont::create("FPS/TPS", "goldFont.fnt");
        titleLabel->setPosition({winSize.width / 2, (winSize.height / 2) + 100});
        backgroundLayer->addChild(titleLabel);

        // Create inner execution interaction menu interface layer
        auto subMenu = CCMenu::create();
        subMenu->setPosition({0, 0});
        backgroundLayer->addChild(subMenu);

        // Standard exit interaction close button configuration sequence
        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(MyOptimizationMenu::onClose)
        );
        closeButton->setPosition({15, 225});
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
        // Natively append structural interface layout display instance onto the running scene grid
        CCDirector::sharedDirector()->getRunningScene()->addChild(this, 100);
    }
};

// ============================================================================
// 2. PLAYLAYER HOOK (Defining a custom class name for proper menu scope resolution)
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

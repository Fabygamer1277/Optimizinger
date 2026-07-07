#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// 1. UI MENU DEFINITION (Inheriting from Geode Popup base class)
// ============================================================================
class MyOptimizationMenu : public Geode::Popup<> {
protected:
    bool setup() override {
        // Set the window title centered at the top
        this->setTitle("FPS/TPS");
        
        // The popup content layout goes here later. It is currently empty.
        return true;
    }

public:
    static MyOptimizationMenu* create() {
        auto ret = new MyOptimizationMenu();
        // Create a standard size container (320 width x 240 height)
        if (ret && ret->initAnchored(320, 240)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ============================================================================
// 2. PLAYLAYER HOOK (To inject the custom interaction button)
// ============================================================================
class $modify(PlayLayer) {
    // Declare the callback action function first so the compiler registers it
    void onMyMenuButton(CCObject* sender) {
        auto menu = MyOptimizationMenu::create();
        menu->show();
    }

    bool init(GJGameLevel* level, bool useReplay, bool dontRunActions) {
        if (!PlayLayer::init(level, useReplay, dontRunActions)) return false;

        // Retrieve the native top-left UI button menu container
        auto uiMenu = this->getChildByID("ui-menu");
        if (!uiMenu) {
            // Fallback layout initialization if the ID wrapper fails
            uiMenu = CCMenu::create();
            uiMenu->setPosition({20, CCDirector::sharedDirector()->getWinSize().height - 60});
            this->addChild(uiMenu);
        }

        // Use a built-in green circle add sprite frame texture asset
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        
        // Link the button element instance execution callback to our action trigger
        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(PlayLayer::onMyMenuButton)
        );

        // Assign a distinct unique node identifier reference
        myButton->setID("fps-optimizer-button");
        
        // Target append node onto layout hierarchy array and update formatting
        uiMenu->addChild(myButton);
        uiMenu->updateLayout();

        return true;
    }
};

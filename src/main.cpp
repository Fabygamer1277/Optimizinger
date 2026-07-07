#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// 1. UI MENU DEFINITION (Inheriting from the correct lowercase geode namespace)
// ============================================================================
class MyOptimizationMenu : public geode::Popup<> {
protected:
    bool setup() override {
        // Set the window title centered at the top natively
        this->setTitle("FPS/TPS");
        
        // This is the empty base container for testing
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
// 2. PLAYLAYER HOOK (Defining a custom class name for proper menu scope resolution)
// ============================================================================
class $modify(MyPlayLayer, PlayLayer) {
    // Declare the interactive button callback action inside the modified scope
    void onMyMenuButton(CCObject* sender) {
        auto menu = MyOptimizationMenu::create();
        menu->show();
    }

    bool init(GJGameLevel* level, bool useReplay, bool dontRunActions) {
        if (!PlayLayer::init(level, useReplay, dontRunActions)) return false;

        // Retrieve the standard top-left UI menu layer layout identifier
        auto uiMenu = this->getChildByID("ui-menu");
        if (!uiMenu) {
            // Backup coordinates fallback configuration structure
            uiMenu = CCMenu::create();
            uiMenu->setPosition({20, CCDirector::sharedDirector()->getWinSize().height - 60});
            this->addChild(uiMenu);
        }

        // Create the green plus button frame from the native game spritesheet
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        
        // Target the custom MyPlayLayer scope handler method for the action selector
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

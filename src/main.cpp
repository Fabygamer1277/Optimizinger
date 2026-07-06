#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class MyOptimizationMenu : public FLAlertLayer {
public:
    static MyOptimizationMenu* create() {
        auto ret = new MyOptimizationMenu();
        if (ret && ret->init(320, 240, "GJ_square01.png", "FPS/TPS")) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(float width, float height, const char* bg, const char* title) {
        if (!FLAlertLayer::init(this, width, height, bg, title)) return false;
        return true;
    }

    void onClose(CCObject* sender) {
        setKeypadEnabled(false);
        removeFromParentAndCleanup(true);
    }
};

class $modify(PlayLayer) {
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
            menu_selector(PlayLayer::onMyMenuButton)
        );

        myButton->setID("fps-optimizer-button");
        
        uiMenu->addChild(myButton);
        uiMenu->updateLayout();

        return true;
    }

    void onMyMenuButton(CCObject* sender) {
        auto menu = MyOptimizationMenu::create();
        menu->show();
    }
};

#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// 1. UI MENU DEFINITION (FLAlertLayer standard base container layout)
// ============================================================================
class MyOptimizationMenu : public FLAlertLayer {
protected:
    bool init() {
        if (!FLAlertLayer::init(150)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Create a standard CCLayer container to hold all visual elements
        auto mainLayer = CCLayer::create();
        this->addChild(mainLayer);
        this->m_mainLayer = mainLayer;

        // Create and add the background texture inside our main layer
        auto backgroundLayer = CCScale9Sprite::create("GJ_square01.png");
        backgroundLayer->setContentSize({320, 240});
        backgroundLayer->setPosition(winSize / 2);
        mainLayer->addChild(backgroundLayer);

        // Setup custom text title centered horizontally near the top of the box
        auto titleLabel = CCLabelBMFont::create("FPS/TPS", "goldFont.fnt");
        titleLabel->setPosition({winSize.width / 2, (winSize.height / 2) + 95});
        mainLayer->addChild(titleLabel);

        // The menu MUST be a direct child of mainLayer to register touches properly
        auto subMenu = CCMenu::create();
        subMenu->setPosition({0, 0});
        mainLayer->addChild(subMenu);

        // Standard close popup window button using a precise top-left offset configuration
        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(MyOptimizationMenu::onClose)
        );
        // Positions the 'X' exactly inside the top-left circle overlay area
        closeButton->setPosition({(winSize.width / 2) - 145, (winSize.height / 2) + 105});
        subMenu->addChild(closeButton);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
    }

    void onClose(CCObject* sender) {
        // Safely remove the user interface overlay layout from the scene tree
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
// 2. PAUSEMENU HOOK (Safe implementation using customSetup for GD 2.2081)
// ============================================================================
class $modify(MyPauseLayer, PauseLayer) {
    void onMyMenuButton(CCObject* sender) {
        auto menu = MyOptimizationMenu::create();
        if (menu) {
            menu->show();
        }
    }

    // Usamos customSetup que es inmune a los problemas de firmas del init en la 2.2081
    void customSetup() {
        // Ejecutamos primero la configuración nativa del juego
        PauseLayer::customSetup();

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Crear contenedor para el botón arriba a la izquierda
        auto topMenu = CCMenu::create();
        topMenu->setPosition({30, winSize.height - 30});
        this->addChild(topMenu);

        // Textura del botón del más (+) verde
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        
        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onMyMenuButton)
        );

        myButton->setID("fps-optimizer-pause-button");
        
        topMenu->addChild(myButton);
        topMenu->updateLayout();
    }
};

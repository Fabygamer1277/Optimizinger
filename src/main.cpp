#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// CLASE DEL MENÚ EMERGENTE
// ============================================================================
class MyOptimizationMenu : public FLAlertLayer {
protected:
    bool init() override {
        if (!FLAlertLayer::init(150)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto mainLayer = CCLayer::create();
        this->addChild(mainLayer);
        this->m_mainLayer = mainLayer;

        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({320, 240});
        bg->setPosition(winSize / 2);
        mainLayer->addChild(bg);

        auto title = CCLabelBMFont::create("FPS/TPS", "goldFont.fnt");
        title->setPosition({winSize.width / 2, (winSize.height / 2) + 95});
        mainLayer->addChild(title);

        auto subMenu = CCMenu::create();
        subMenu->setPosition({0, 0});
        subMenu->setTouchPriority(-501);
        mainLayer->addChild(subMenu);

        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(closeSprite, this, menu_selector(MyOptimizationMenu::onClose));
        closeBtn->setPosition({(winSize.width / 2) - 145, (winSize.height / 2) + 105});
        subMenu->addChild(closeBtn);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);
        return true;
    }

    void registerWithTouchDispatcher() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
    }

    bool ccTouchBegan(CCTouch* t, CCEvent* e) override { return true; }
    void ccTouchMoved(CCTouch* t, CCEvent* e) override {}
    void ccTouchEnded(CCTouch* t, CCEvent* e) override {}
    void onClose(CCObject* s) { this->removeFromParentAndCleanup(true); }

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
    
    void show() override {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (scene) scene->addChild(this, 1000);
    }
};

// ============================================================================
// HOOK DE PAUSELAYER (Código completo)
// ============================================================================
class $modify(MyPauseLayer, PauseLayer) {
    void onMyMenuButton(CCObject* sender) {
        auto menu = MyOptimizationMenu::create();
        if (menu) menu->show();
    }

    void customSetup() {
        PauseLayer::customSetup();

        // Creamos un menú propio fuera del layout automático de Geode
        auto myMenu = CCMenu::create();
        myMenu->setID("fps-optimizer-manual-menu");
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // POSICIÓN: Aquí ajustas el número '45.0f' para alinear con las líneas rojas
        myMenu->setPosition({45.0f, winSize.height - 75.0f});
        this->addChild(myMenu, 100);

        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        buttonSprite->setScale(0.75f); 

        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite, 
            this, 
            menu_selector(MyPauseLayer::onMyMenuButton)
        );
        
        myMenu->addChild(myButton);
    }
};

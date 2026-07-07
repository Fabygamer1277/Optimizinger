#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// 1. UI MENU DEFINITION (Strict Modal Touch Isolation for Android)
// ============================================================================
class MyOptimizationMenu : public FLAlertLayer {
protected:
    bool init() override {
        if (!FLAlertLayer::init(150)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Contenedor principal de la interfaz
        auto mainLayer = CCLayer::create();
        this->addChild(mainLayer);
        this->m_mainLayer = mainLayer;

        // Fondo del recuadro café del menú
        auto backgroundLayer = CCScale9Sprite::create("GJ_square01.png");
        backgroundLayer->setContentSize({320, 240});
        backgroundLayer->setPosition(winSize / 2);
        mainLayer->addChild(backgroundLayer);

        // Título del menú
        auto titleLabel = CCLabelBMFont::create("FPS/TPS", "goldFont.fnt");
        titleLabel->setPosition({winSize.width / 2, (winSize.height / 2) + 95});
        mainLayer->addChild(titleLabel);

        // Menú exclusivo para los botones internos del panel
        auto subMenu = CCMenu::create();
        subMenu->setPosition({0, 0});
        subMenu->setTouchPriority(-501); // Prioridad ultra alta sobre el escudo
        mainLayer->addChild(subMenu);

        // Botón de cerrar (X)
        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(MyOptimizationMenu::onClose)
        );
        closeButton->setPosition({(winSize.width / 2) - 145, (winSize.height / 2) + 105});
        subMenu->addChild(closeButton);

        // Activamos los toques de forma estricta
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
    }

    // Registramos este panel en el despachador de toques con prioridad de bloqueo crítico (-500)
    void registerWithTouchDispatcher() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
    }

    // BLOQUEO TOTAL TRASERO: Absorbe cualquier interacción de fondo
    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        return true; 
    }

    void ccTouchMoved(CCTouch* touch, CCEvent* event) override {}
    void ccTouchEnded(CCTouch* touch, CCEvent* event) override {}

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
    
    void show() override {
        auto runningScene = CCDirector::sharedDirector()->getRunningScene();
        if (runningScene) {
            runningScene->addChild(this, 1000); // Renderizado superior absoluto
        }
    }
};

// ============================================================================
// 2. PAUSEMENU HOOK (Geode v5 Modern Dynamic Engine)
// ============================================================================
class $modify(MyPauseLayer, PauseLayer) {
    void onMyMenuButton(CCObject* sender) {
        auto menu = MyOptimizationMenu::create();
        if (menu) {
            menu->show();
        }
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        CCMenu* targetMenu = nullptr;
        
        // CORRECCIÓN GEODE v5: Iteración moderna usando getChildrenExt()
        for (auto child : this->getChildrenExt()) {
            auto menu = dynamic_cast<CCMenu*>(child);
            if (menu) {
                // Filtramos si el menú está posicionado en la esquina superior izquierda
                if (menu->getPositionX() < 100 && menu->getPositionY() > (winSize.height - 100)) {
                    targetMenu = menu;
                    break;
                }
            }
        }

        auto topMenu = CCMenu::create();
        this->addChild(topMenu, 150);

        // Textura del botón verde (+)
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        
        // REESCALADO: Tamaño de tuerca idéntico al botón de configuración nativo
        buttonSprite->setScale(0.75f); 

        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onMyMenuButton)
        );
        myButton->setID("fps-optimizer-pause-button");
        topMenu->addChild(myButton);

        // LÓGICA DE ESQUIVE RESPONSIVO (Soporte Geode v5)
        if (targetMenu && targetMenu->getChildrenCount() > 0) {
            float bottomOffset = winSize.height - 30;
            
            for (auto btnChild : targetMenu->getChildrenExt()) {
                float nodeBottom = targetMenu->getPositionY() + btnChild->getPositionY() - (btnChild->getContentSize().height / 2);
                if (nodeBottom < bottomOffset) {
                    bottomOffset = nodeBottom;
                }
            }
            // Desplazamiento dinámico seguro dejando margen estético
            topMenu->setPosition({30, bottomOffset - 20});
        } else {
            // Posición por defecto si la esquina superior izquierda está libre
            topMenu->setPosition({30, winSize.height - 30});
        }

        topMenu->updateLayout();
    }
};

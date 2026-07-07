#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// 1. UI MENU DEFINITION (Panel de optimización con aislamiento táctil)
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
// 2. PAUSEMENU HOOK (Inyección Limpia en el Menú Nativo de Esquina)
// ============================================================================
class $modify(MyPauseLayer, PauseLayer) {
    void onMyMenuButton(CCObject* sender) {
        auto menu = MyOptimizationMenu::create();
        if (menu) {
            menu->show();
        }
    }

    void customSetup() {
        // Ejecutamos la carga base del juego primero
        PauseLayer::customSetup();

        // Creamos la textura de la tuerca/más (+) verde reescalado
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        buttonSprite->setScale(0.75f); // Tamaño idéntico a la tuerca estándar

        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onMyMenuButton)
        );
        myButton->setID("fps-optimizer-pause-button");

        // BUSQUEDA DIRECTA POR ID NATIVO: Obtenemos el contenedor izquierdo oficial del PauseLayer
        auto leftMenu = this->getChildByID("left-menu");
        
        if (leftMenu) {
            // Si Geode/GD tienen el contenedor activo, metemos el botón ahí directamente
            leftMenu->addChild(myButton);
            // Forzamos a que el Layout recalcule las posiciones verticales automáticamente
            leftMenu->updateLayout();
        } else {
            // Plan de respaldo de emergencia si el layout nativo no responde
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto fallbackMenu = CCMenu::create();
            fallbackMenu->setPosition({30.0f, winSize.height - 75.0f}); // Forzado abajo por defecto
            this->addChild(fallbackMenu, 150);
            fallbackMenu->addChild(myButton);
            fallbackMenu->updateLayout();
        }
    }
};

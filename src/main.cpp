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
// 2. PAUSEMENU HOOK (Algoritmo de Esquive Dinámico Inteligente)
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
        
        // Creamos nuestro contenedor propio para el botón (+)
        auto topMenu = CCMenu::create();
        this->addChild(topMenu, 150);

        // Textura y configuración del botón verde (+)
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        buttonSprite->setScale(0.75f); // Tamaño exacto al botón de configuración nativo

        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onMyMenuButton)
        );
        myButton->setID("fps-optimizer-pause-button");
        topMenu->addChild(myButton);

        // --------------------------------------------------------------------
        // DETECCIÓN DINÁMICA DE COMPETENCIA DE MENÚS (ESQUIVE)
        // --------------------------------------------------------------------
        float lowestYFound = winSize.height - 30.0f;
        bool conflictDetected = false;

        // Intento 1: Buscar directamente por el ID del menú del Death Tracker si existe
        auto deathTrackerMenu = this->getChildByID("death-tracker-menu"); // ID común o similar
        
        // Intento 2: Escaneo de nodos en la esquina superior izquierda (Soporte Geode v5)
        for (auto child : this->getChildrenExt()) {
            auto menu = dynamic_cast<CCMenu*>(child);
            if (menu && menu != topMenu) {
                // Si el menú está ubicado en la franja izquierda superior
                if (menu->getPositionX() < 120.0f && menu->getPositionY() > (winSize.height - 120.0f)) {
                    
                    // Analizamos la posición real más baja de sus elementos hijos
                    for (auto btnChild : menu->getChildrenExt()) {
                        float absoluteChildY = menu->getPositionY() + btnChild->getPositionY();
                        float childBottomEdge = absoluteChildY - (btnChild->getContentSize().height * btnChild->getScale() / 2.0f);
                        
                        if (childBottomEdge < lowestYFound) {
                            lowestYFound = childBottomEdge;
                            conflictDetected = true;
                        }
                    }
                }
            }
        }

        // Asignación de posición final responsiva
        if (conflictDetected) {
            // Si hay un mod arriba, nos colocamos 25 unidades abajo de su borde inferior
            topMenu->setPosition({30.0f, lowestYFound - 25.0f});
        } else {
            // Si la esquina está completamente limpia, nos colocamos arriba de forma nativa
            topMenu->setPosition({30.0f, winSize.height - 30.0f});
        }

        topMenu->updateLayout();
    }
};

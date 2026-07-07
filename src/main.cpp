#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// ============================================================================
// 1. UI MENU DEFINITION (Strict Modal Touch Isolation for Android)
// ============================================================================
class MyOptimizationMenu : public FLAlertLayer {
protected:
    bool init() {
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
        // Le damos prioridad máxima absoluta al contenedor de nuestros botones internos
        subMenu->setTouchPriority(-501); 
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

    // BLOQUEO TOTAL TRASERO: Esta función intercepta cualquier toque en la pantalla
    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        // Retornar true le dice a Cocos2d-x que nosotros "nos adueñamos" del toque 
        // y gracias al parámetro 'true' en el dispatcher, el evento se TRAGA y no pasa al fondo.
        return true; 
    }

    void ccTouchMoved(CCTouch* touch, CCEvent* event) override {
        // Bloquea también los arrastres de dedos por la pantalla trasera
    }

    void ccTouchEnded(CCTouch* touch, CCEvent* event) override {
        // Bloquea la liberación del toque trasero
    }

    void onClose(CCObject* sender) {
        // Al cerrar, liberamos la memoria y el juego vuelve a la normalidad automáticamente
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
        auto runningScene = CCDirector::sharedDirector()->getRunningScene();
        if (runningScene) {
            runningScene->addChild(this, 1000); // Se dibuja al frente de absolutamente todo
        }
    }
};

// ============================================================================
// 2. PAUSEMENU HOOK (Dynamic Positioning & Resize)
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

        // Buscamos el menú superior izquierdo nativo/modificado del juego
        CCMenu* targetMenu = nullptr;
        
        CCObject* child;
        CCARRAY_FOREACH(this->getChildren(), child) {
            auto menu = dynamic_cast<CCMenu*>(child);
            if (menu) {
                if (menu->getPositionX() < 100 && menu->getPositionY() > (winSize.height - 100)) {
                    targetMenu = menu;
                    break;
                }
            }
        }

        auto topMenu = CCMenu::create();
        this->addChild(topMenu, 150);

        // Textura del más (+) verde
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        
        // Ajustamos la escala para igualar el tamaño de la tuerca nativa de configuración
        buttonSprite->setScale(0.75f); 

        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onMyMenuButton)
        );
        myButton->setID("fps-optimizer-pause-button");
        topMenu->addChild(myButton);

        // Lógica de desplazamiento dinámico responsivo si hay otro mod en la esquina
        if (targetMenu && targetMenu->getChildrenCount() > 0) {
            float bottomOffset = winSize.height - 30;
            
            CCObject* btnChild;
            CCARRAY_FOREACH(targetMenu->getChildren(), btnChild) {
                auto node = dynamic_cast<CCNode*>(btnChild);
                if (node) {
                    float nodeBottom = targetMenu->getPositionY() + node->getPositionY() - (node->getContentSize().height / 2);
                    if (nodeBottom < bottomOffset) {
                        bottomOffset = nodeBottom;
                    }
                }
            }
            // Coloca el botón abajo del mod existente con un margen estético
            topMenu->setPosition({30, bottomOffset - 20});
        } else {
            // Posición original limpia por defecto si la esquina está libre
            topMenu->setPosition({30, winSize.height - 30});
        }

        topMenu->updateLayout();
    }
};

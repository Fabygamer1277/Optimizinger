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

    // Registramos

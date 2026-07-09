#include "MyOptimizationMenu.hpp"

using namespace cocos2d;

MyOptimizationMenu* MyOptimizationMenu::create(std::string const& value) {
    auto ret = new MyOptimizationMenu();
    // geode::Popup usa un tamaño por defecto en su create (ej: 240x160)
    if (ret && ret->initAnchored(360.f, 240.f, value)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::setup(std::string const& value) {
    // Definimos el título usando el método nativo de Geode::Popup
    this->setTitle("FPS OPTIMIZER", "goldFont.fnt", 0.85f);

    auto menuSize = m_buttonMenu->getContentSize();

    // 1. Ocultar el fondo marrón por defecto e integrar el tuyo morado
    if (m_bgSprite) {
        m_bgSprite->setVisible(false);
    }

    auto bgMorado = CCScale9Sprite::create("menu_purple.png"_spr);
    if (bgMorado) {
        bgMorado->setContentSize(m_size);
        bgMorado->setPosition(m_size / 2);
        m_mainLayer->addChild(bgMorado, -1);
    }

    // 2. Botón de cerrar (Lo movemos a la esquina superior izquierda del Popup)
    auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSprite, this, menu_selector(MyOptimizationMenu::onClose)
    );
    closeBtn->setPosition({ -m_size.width / 2 + 15.f, m_size.height / 2 - 15.f });
    m_buttonMenu->addChild(closeBtn);

    // 3. Textos e Interfaz interna
    auto tpsLabel = CCLabelBMFont::create("Ticks per Second (TPS):", "bigFont.fnt");
    tpsLabel->setScale(0.4f);
    tpsLabel->setPosition({ m_size.width / 2, m_size.height / 2 + 30.f });
    m_mainLayer->addChild(tpsLabel);

    // Texto del valor actual (Cargado desde guardado)
    int currentTPS = geode::Mod::get()->getSavedValue<int>("tps", 60);
    std::string tpsStr = std::to_string(currentTPS);
    
    auto tpsValue = CCLabelBMFont::create(tpsStr.c_str(), "bigFont.fnt");
    tpsValue->setScale(0.5f);
    tpsValue->setColor({ 0, 255, 0 });
    tpsValue->setPosition({ m_size.width / 2, m_size.height / 2 });
    m_mainLayer->addChild(tpsValue);

    return true;
}

void MyOptimizationMenu::onClose(CCObject* sender) {
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
}

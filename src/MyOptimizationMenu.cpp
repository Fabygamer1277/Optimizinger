#include "MyOptimizationMenu.hpp"
#include <cocos-ext.h> // <- Necesario para CCScale9Sprite

using namespace cocos2d;
using namespace cocos2d::extension; // <- Espacio de nombres para los sprites de escala 9

MyOptimizationMenu* MyOptimizationMenu::create(std::string const& value) {
    auto ret = new MyOptimizationMenu();
    if (ret && ret->initAnchored(360.f, 240.f, value)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::setup(std::string const& value) {
    // Definimos el título nativo
    this->setTitle("FPS OPTIMIZER");

    // Ocultar el fondo marrón original
    if (this->m_bgSprite) {
        this->m_bgSprite->setVisible(false);
    }

    // Cargar tu fondo personalizado morado usando el helper de Geode
    auto bgMorado = CCScale9Sprite::create("menu_purple.png"_spr);
    if (bgMorado) {
        bgMorado->setContentSize(this->m_size);
        bgMorado->setPosition(this->m_size / 2);
        this->m_mainLayer->addChild(bgMorado, -1);
    }

    // Reposicionar de manera segura el botón de cerrar nativo de Geode
    if (this->m_closeBtn) {
        this->m_closeBtn->setPosition({ -this->m_size.width / 2 + 15.f, this->m_size.height / 2 - 15.f });
    }

    // Interfaz interna: Etiquetas de texto
    auto tpsLabel = CCLabelBMFont::create("Ticks per Second (TPS):", "bigFont.fnt");
    tpsLabel->setScale(0.4f);
    tpsLabel->setPosition({ this->m_size.width / 2, this->m_size.height / 2 + 30.f });
    this->m_mainLayer->addChild(tpsLabel);

    // Obtener valor actual guardado en Geode
    int currentTPS = geode::Mod::get()->getSavedValue<int>("tps", 60);
    std::string tpsStr = std::to_string(currentTPS);
    
    auto tpsValue = CCLabelBMFont::create(tpsStr.c_str(), "bigFont.fnt");
    tpsValue->setScale(0.5f);
    tpsValue->setColor({ 0, 255, 0 });
    tpsValue->setPosition({ this->m_size.width / 2, this->m_size.height / 2 });
    this->m_mainLayer->addChild(tpsValue);

    return true;
}

void MyOptimizationMenu::onClose(CCObject* sender) {
    // Delegamos el cierre seguro a la clase base de Geode para evitar fugas de memoria
    geode::Popup<std::string const&>::onClose(sender);
}

#include "MyOptimizationMenu.hpp"

MyOptimizationMenu* MyOptimizationMenu::create() {
    auto ret = new MyOptimizationMenu();
    // Definimos un tamaño estándar de 380x260 y el título interno
    if (ret && ret->initAnchored(380.f, 260.f, "OPTIMIZER OPTIONS")) { 
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::setup(std::string const& title) {
    // El título se asigna automáticamente mediante initAnchored
    this->setTitle(title);

    // Ocultar fondo por defecto si se requiere personalización
    if (m_bgSprite) m_bgSprite->setVisible(false);

    // Crear un fondo personalizado adaptado a m_size
    auto customBg = CCScale9Sprite::create("GJ_square01.png");
    if (customBg) {
        customBg->setContentSize(m_size);
        customBg->setPosition(m_size / 2);
        m_mainLayer->addChild(customBg, -1);
    }

    // Alinear el botón de cierre en la esquina superior izquierda usando m_size
    if (m_closeBtn) {
        m_closeBtn->setPosition({ -m_size.width / 2 + 20.f, m_size.height / 2 - 20.f });
    }

    // Texto indicador para los TPS
    auto tpsLabel = CCLabelBMFont::create("TPS Limit:", "bigFont.fnt");
    if (tpsLabel) {
        tpsLabel->setPosition({ m_size.width / 2 - 80.f, m_size.height / 2 + 40.f });
        m_mainLayer->addChild(tpsLabel);
    }

    // Crear la caja de entrada de texto
    auto m_tpsInput = TextInput::create(120.f, "60", "bigFont.fnt");
    if (m_tpsInput) {
        m_tpsInput->setPosition({ m_size.width / 2 + 40.f, m_size.height / 2 + 40.f });
        
        // Bloqueamos para que solo acepte caracteres numéricos
        m_tpsInput->setAllowedChars("0123456789");
        
        m_mainLayer->addChild(m_tpsInput);
    }

    return true;
}

#include "MyOptimizationMenu.hpp"

MyOptimizationMenu* MyOptimizationMenu::create(std::string const& title) {
    auto ret = new MyOptimizationMenu();
    // initAnchored inicializa las dimensiones (380 de ancho, 260 de alto) y le pasa el título a la clase base
    if (ret && ret->initAnchored(380.f, 260.f, title)) { 
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::setup(std::string const& title) {
    // El título se asigna automáticamente mediante la clase base geode::Popup,
    // pero nos aseguramos de setearlo correctamente aquí:
    this->setTitle(title);

    // Ocultar el fondo por defecto si es necesario
    if (m_bgSprite) m_bgSprite->setVisible(false);

    // Crear un fondo personalizado adaptado a m_size
    auto customBg = CCScale9Sprite::create("GJ_square01.png");
    if (customBg) {
        customBg->setContentSize(m_size);
        customBg->setPosition(m_size / 2);
        m_mainLayer->addChild(customBg, -1);
    }

    // Reposicionar el botón de cierre en la esquina superior izquierda usando m_size
    if (m_closeBtn) {
        m_closeBtn->setPosition({ -m_size.width / 2 + 20.f, m_size.height / 2 - 20.f });
    }

    // Etiqueta indicadora para el input de TPS
    auto tpsLabel = CCLabelBMFont::create("TPS Limit:", "bigFont.fnt");
    if (tpsLabel) {
        tpsLabel->setPosition({ m_size.width / 2 - 80.f, m_size.height / 2 + 40.f });
        m_mainLayer->addChild(tpsLabel);
    }

    // Crear la caja de entrada de texto usando el TextInput de Geode
    // Nota: Reemplaza "m_tpsInput" con tu variable miembro si la declaraste en el .hpp, 
    // o déjala como una variable local/miembro según la tengas estructurada en tu mod.
    auto m_tpsInput = TextInput::create(120.f, "60", "bigFont.fnt");
    if (m_tpsInput) {
        m_tpsInput->setPosition({ m_size.width / 2 + 40.f, m_size.height / 2 + 40.f });
        
        // SOLUCIÓN AL PASO 2: En lugar de usar TextInputFilter que no existe,
        // le indicamos los únicos caracteres permitidos (solo números)
        m_tpsInput->setAllowedChars("0123456789");
        
        m_mainLayer->addChild(m_tpsInput);
    }

    return true;
}

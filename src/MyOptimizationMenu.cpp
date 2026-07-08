#include "MyOptimizationMenu.hpp"

MyOptimizationMenu* MyOptimizationMenu::create() {
    auto ret = new MyOptimizationMenu();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::init() {
    // Inicializamos la alerta vacía (0) para usar tus imágenes personalizadas
    if (!FLAlertLayer::init(0)) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 1. Contenedor de botones
    auto menu = CCMenu::create();
    m_mainLayer->addChild(menu);

    // 2. Fondo personalizado de tu menú (menu_purple.png)
    auto customBg = CCSprite::create("menu_purple.png");
    if (customBg) {
        customBg->setPosition(winSize / 2);
        customBg->setScale(0.55f); 
        m_mainLayer->addChild(customBg, -1);
    }

    // 3. Título del menú superior
    auto title = CCLabelBMFont::create("FPS OPTIMIZER", "goldFont.fnt");
    title->setPosition({ winSize.width / 2, (winSize.height / 2) + 85.f });
    title->setScale(0.75f);
    m_mainLayer->addChild(title);

    // 4. Botón de cerrar (X)
    auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSprite, this, menu_selector(MyOptimizationMenu::onClose)
    );
    closeBtn->setPosition({ -135.f, 85.f });
    menu->addChild(closeBtn);

    // =================================================================
    // SECCIÓN 1: CONFIGURACIÓN DE TPS (Fila Superior)
    // =================================================================
    
    // Texto indicativo
    auto tpsLabel = CCLabelBMFont::create("Ticks per Second (TPS):", "bigFont.fnt");
    tpsLabel->setPosition({ winSize.width / 2 - 40.f, winSize.height / 2 + 35.f });
    tpsLabel->setScale(0.35f);
    m_mainLayer->addChild(tpsLabel);

    // Primera copia de box_values.png como contenedor visual
    auto tpsBoxBg = CCSprite::create("box_values.png");
    if (tpsBoxBg) {
        tpsBoxBg->setPosition({ winSize.width / 2 + 75.f, winSize.height / 2 + 35.f });
        tpsBoxBg->setScaleX(0.25f); // Ajustamos el ancho para que quepa el número
        tpsBoxBg->setScaleY(0.4f);
        m_mainLayer->addChild(tpsBoxBg);
    }

    // Caja de entrada de texto interactiva para TPS
    m_tpsInput = CCTextInputNode::create(60.f, 20.f, "1000", "bigFont.fnt");
    m_tpsInput->setPosition({ winSize.width / 2 + 75.f, winSize.height / 2 + 35.f });
    m_tpsInput->setDelegate(this);
    m_tpsInput->setAllowedChars("0123456789"); // Evita que metan letras
    m_tpsInput->setMaxLabelLength(4);          // Máximo 4 dígitos (hasta 2000)
    m_tpsInput->setScale(0.5f);
    m_mainLayer->addChild(m_tpsInput);

    // =================================================================
    // SECCIÓN 2: CONFIGURACIÓN DE FPS (Fila Inferior)
    // =================================================================
    
    // Texto indicativo
    auto fpsLabel = CCLabelBMFont::create("Visual Frames (FPS):", "bigFont.fnt");
    fpsLabel->setPosition({ winSize.width / 2 - 45.f, winSize.height / 2 - 15.f });
    fpsLabel->setScale(0.35f);
    m_mainLayer->addChild(fpsLabel);

    // Segunda copia de box_values.png como contenedor visual
    auto fpsBoxBg = CCSprite::create("box_values.png");
    if (fpsBoxBg) {
        fpsBoxBg->setPosition({ winSize.width / 2 + 75.f, winSize.height / 2 - 15.f });
        fpsBoxBg->setScaleX(0.25f);
        fpsBoxBg->setScaleY(0.4f);
        m_mainLayer->addChild(fpsBoxBg);
    }

    // Caja de entrada de texto interactiva para FPS
    m_fpsInput = CCTextInputNode::create(60.f, 20.f, "60", "bigFont.fnt");
    m_fpsInput->setPosition({ winSize.width / 2 + 75.f, winSize.height / 2 - 15.f });
    m_fpsInput->setDelegate(this);
    m_fpsInput->setAllowedChars("0123456789");
    m_fpsInput->setMaxLabelLength(3);          // Máximo 3 dígitos (hasta 540)
    m_fpsInput->setScale(0.5f);
    m_mainLayer->addChild(m_fpsInput);

    return true;
}

// Filtro en tiempo real para hacer cumplir los rangos numéricos que solicitaste
void MyOptimizationMenu::textChanged(CCTextInputNode* input) {
    std::string valueStr = input->getString();
    if (valueStr.empty()) return;

    int value = std::stoi(valueStr);

    if (input == m_tpsInput) {
        // Rango TPS: Mayor a 30 y Menor a 2000
        if (value > 2000) input->setString("2000");
        // Nota: La validación del mínimo de 30 se hace al cerrar o perder enfoque para dejar escribir
    } 
    else if (input == m_fpsInput) {
        // Rango FPS: Mayor a 60 y Menor a 540
        if (value > 540) input->setString("540");
    }
}

void MyOptimizationMenu::onClose(cocos2d::CCObject* sender) {
    // Validaciones finales de mínimos antes de guardar e irse
    if (!m_tpsInput->getString().empty() && std::stoi(m_tpsInput->getString()) < 30) {
        m_tpsInput->setString("30");
    }
    if (!m_fpsInput->getString().empty() && std::stoi(m_fpsInput->getString()) < 60) {
        m_fpsInput->setString("60");
    }

    // Aquí puedes aplicar los valores reales al motor usando tu lógica de bypass o CBF de Geode

    this->keyBackClicked(); // Cierre seguro de la alerta flotante
}

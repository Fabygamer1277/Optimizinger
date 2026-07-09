#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

MyOptimizationMenu* MyOptimizationMenu::create() {
    auto ret = new MyOptimizationMenu();
    if (ret && ret->initAnchored(380.f, 260.f)) { 
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::setup() {
    if (m_bgSprite) m_bgSprite->setVisible(false);

    // Fondo morado personalizado
    auto customBg = CCSprite::create("menu_purple.png"_spr);
    if (customBg) {
        customBg->setContentSize(m_size);
        customBg->setPosition(m_size / 2);
        m_mainLayer->addChild(customBg, -1);
    }

    this->setTitle("OPTIMIZER OPTIONS");

    if (m_closeBtn) {
        m_closeBtn->setPosition({ -m_size.width / 2 + 20.f, m_size.height / 2 - 20.f });
    }

    // --- SECCIÓN TPS ---
    // CORREGIDO: Llamamos a tu fuente usando el ID del mod + el nombre que le diste en el json
    auto tpsLabel = CCLabelBMFont::create("Target TPS:", "fabygamer1277.fps-optimizer/gothic-font.fnt");
    if (tpsLabel) {
        tpsLabel->setScale(0.50f); // Bajamos un pelín la escala por ser fuente gótica externa
        tpsLabel->setPosition({ m_size.width / 2 - 80.f, m_size.height / 2 + 40.f });
        m_mainLayer->addChild(tpsLabel);
    }

    int currentTPS = Mod::get()->getSavedValue<int>("tps", 60);
    // CORREGIDO: Aplicamos tu fuente gótica a la caja de texto
    m_tpsInput = TextInput::create(120.f, "60", "fabygamer1277.fps-optimizer/gothic-font.fnt");
    if (m_tpsInput) {
        m_tpsInput->setString(std::to_string(currentTPS));
        m_tpsInput->setFilter(TextInputFilter::create(true, false)); 
        m_tpsInput->setPosition({ m_size.width / 2 + 50.f, m_size.height / 2 + 40.f });
        
        if (auto bgInput = m_tpsInput->getBGSprite()) {
            auto customBox = CCSprite::create("box_values.png"_spr);
            if (customBox) {
                customBox->setContentSize(bgInput->getContentSize());
                customBox->setPosition(bgInput->getPosition());
                bgInput->getParent()->addChild(customBox, bgInput->getZOrder());
                bgInput->removeFromParent(); 
            }
        }
        m_mainLayer->addChild(m_tpsInput);
    }

    // --- SECCIÓN FPS ---
    auto fpsLabel = CCLabelBMFont::create("Target FPS:", "fabygamer1277.fps-optimizer/gothic-font.fnt");
    if (fpsLabel) {
        fpsLabel->setScale(0.50f);
        fpsLabel->setPosition({ m_size.width / 2 - 80.f, m_size.height / 2 - 10.f });
        m_mainLayer->addChild(fpsLabel);
    }

    int currentFPS = Mod::get()->getSavedValue<int>("fps", 60);
    m_fpsInput = TextInput::create(120.f, "60", "fabygamer1277.fps-optimizer/gothic-font.fnt");
    if (m_fpsInput) {
        m_fpsInput->setString(std::to_string(currentFPS));
        m_fpsInput->setFilter(TextInputFilter::create(true, false));
        m_fpsInput->setPosition({ m_size.width / 2 + 50.f, m_size.height / 2 - 10.f });
        
        if (auto bgInput = m_fpsInput->getBGSprite()) {
            auto customBox = CCSprite::create("box_values.png"_spr);
            if (customBox) {
                customBox->setContentSize(bgInput->getContentSize());
                customBox->setPosition(bgInput->getPosition());
                bgInput->getParent()->addChild(customBox, bgInput->getZOrder());
                bgInput->removeFromParent();
            }
        }
        m_mainLayer->addChild(m_fpsInput);
    }

    // --- BOTÓN DE GUARDAR ---
    auto btnSprite = ButtonSprite::create("Guardar", "fabygamer1277.fps-optimizer/gothic-font.fnt", "GJ_button_01.png");
    auto saveBtn = CCMenuItemSpriteExtra::create(
        btnSprite,
        this,
        menu_selector(MyOptimizationMenu::onSave)
    );
    
    auto menuBtn = CCMenu::create();
    if (menuBtn && saveBtn) {
        saveBtn->setPosition({ 0, -m_size.height / 2 + 35.f });
        menuBtn->setPosition({ m_size.width / 2, m_size.height / 2 });
        menuBtn->addChild(saveBtn);
        m_mainLayer->addChild(menuBtn);
    }

    return true;
}

void MyOptimizationMenu::onSave(cocos2d::CCObject* sender) {
    if (m_tpsInput && m_fpsInput) {
        int tps = m_tpsInput->getString().length() > 0 ? std::stoi(m_tpsInput->getString()) : 60;
        int fps = m_fpsInput->getString().length() > 0 ? std::stoi(m_fpsInput->getString()) : 60;

        // Limites de seguridad basados en tu mod.json actual
        if (tps < 30) tps = 30;
        if (tps > 2000) tps = 2000;
        if (fps < 60) fps = 60;
        if (fps > 540) fps = 540;

        Mod::get()->setSavedValue("tps", tps);
        Mod::get()->setSavedValue("fps", fps);

        FLAlertLayer::create("Guardado", "Configuración de rendimiento actualizada.", "OK")->show();
        this->onClose(sender);
    }
}

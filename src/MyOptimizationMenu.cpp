#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

MyOptimizationMenu* MyOptimizationMenu::create(std::string const& value) {
    auto ret = new MyOptimizationMenu();
    if (ret && ret->initAnchored(360.f, 240.f, value)) { 
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// Modificado: Ahora recibe 'std::string value' por valor para coincidir con el .hpp
bool MyOptimizationMenu::setup(std::string value) {
    this->setTitle("FPS OPTIMIZER");

    if (m_bgSprite) {
        m_bgSprite->setVisible(false);
    }

    auto bgMorado = CCScale9Sprite::create("GJ_square06.png");
    if (bgMorado) {
        bgMorado->setContentSize(m_size);
        bgMorado->setPosition(m_size / 2);
        m_mainLayer->addChild(bgMorado, -1);
    }

    if (m_closeBtn) {
        m_closeBtn->setPosition({ -m_size.width / 2 + 15.f, m_size.height / 2 - 15.f });
    }

    auto tpsLabel = CCLabelBMFont::create("Ticks Por Segundo (TPS)", "goldFont.fnt");
    tpsLabel->setScale(0.7f);
    tpsLabel->setPosition({ m_size.width / 2, m_size.height / 2 + 30.f });
    m_mainLayer->addChild(tpsLabel);

    auto tpsValue = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
    tpsValue->setScale(0.8f);
    tpsValue->setPosition({ m_size.width / 2, m_size.height / 2 });
    m_mainLayer->addChild(tpsValue);

    return true;
}

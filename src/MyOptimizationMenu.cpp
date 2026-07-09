#include "MyOptimizationMenu.hpp"
#include <cocos-ext.h>

using namespace cocos2d;
using namespace cocos2d::extension;

MyOptimizationMenu* MyOptimizationMenu::create(std::string const& value) {
    auto ret = new MyOptimizationMenu();
    ret->m_value = value; // Guardamos el valor antes de inicializar si se ocupa
    
    if (ret && ret->initAnchored(360.f, 240.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::setup() {
    this->setTitle("FPS OPTIMIZER");

    if (m_bgSprite) {
        m_bgSprite->setVisible(false);
    }

    auto bgMorado = CCScale9Sprite::create("menu_purple.png"_spr);
    if (bgMorado) {
        bgMorado->setContentSize(m_size);
        bgMorado->setPosition(m_size / 2);
        m_mainLayer->addChild(bgMorado, -1);
    }

    if (m_closeBtn) {
        m_closeBtn->setPosition({ -m_size.width / 2 + 15.f, m_size.height / 2 - 15.f });
    }

    // Interfaz del menú
    auto tpsLabel = CCLabelBMFont::create("Ticks per Second (TPS):", "bigFont.fnt");
    tpsLabel->setScale(0.4f);
    tpsLabel->setPosition({ m_size.width / 2, m_size.height / 2 + 30.f });
    m_mainLayer->addChild(tpsLabel);

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
    geode::Popup<>::onClose(sender);
}

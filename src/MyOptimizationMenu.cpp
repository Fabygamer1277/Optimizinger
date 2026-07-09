#include "MyOptimizationMenu.hpp"

using namespace cocos2d;

MyOptimizationMenu* MyOptimizationMenu::create(std::string const& value) {
    auto ret = new MyOptimizationMenu();
    // geode::Popup usa initAnchored en lugar de init básico
    if (ret && ret->initAnchored(360.f, 240.f, value)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::setup(std::string const& value) {
    // CORRECCIÓN 1: En Geode,setTitle solo acepta el string del texto. 
    // Si quieres cambiar la fuente o escala, se hace modificando el nodo m_title.
    this->setTitle("FPS OPTIMIZER");
    if (m_title) {
        m_title->setScale(0.85f);
        // Opcional: Si quieres cambiar la fuente del título nativo, tendrías que recrearlo, 
        // pero por defecto Geode usa goldFont.fnt, así que solo ajustar la escala basta.
    }

    // 1. Ocultar el fondo marrón por defecto e integrar el tuyo morado
    if (m_bgSprite) {
        m_bgSprite->setVisible(false);
    }

    // CORRECCIÓN 2: Para usar el operador "_spr" de Geode, necesitas usar la estructura correcta.
    // Si da problemas en compilación, puedes usar geode::Mod::get()->expandSpriteName("menu_purple.png")
    using namespace geode::modifier; // Asegura soporte de literales de Geode si los usas
    auto bgMorado = CCScale9Sprite::create("menu_purple.png"_spr);
    if (bgMorado) {
        bgMorado->setContentSize(m_size);
        bgMorado->setPosition(m_size / 2);
        m_mainLayer->addChild(bgMorado, -1);
    }

    // CORRECCIÓN 3: ¡Geode::Popup ya crea un botón de cerrar automáticamente! 
    // Si creas uno nuevo manualmente en m_buttonMenu, se encimará o tendrás dos.
    // Modificamos el que Geode trae por defecto (m_closeBtn):
    if (m_closeBtn) {
        m_closeBtn->setPosition({ -m_size.width / 2 + 15.f, m_size.height / 2 - 15.f });
    }

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

// CORRECCIÓN 4: Geode::Popup ya implementa su propio cierre seguro al presionar m_closeBtn.
// Redirigimos tu función personalizada para llamar al cierre nativo y evitar crashes de memoria.
void MyOptimizationMenu::onClose(CCObject* sender) {
    geode::Popup<std::string const&>::onClose(sender);
}

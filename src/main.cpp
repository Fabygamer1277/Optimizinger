#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables globales para la configuración
static float g_targetTPS = 60.0f;
static float g_targetFPS = 60.0f;
static bool  g_syncEnabled = false;

class MyOptimizationMenu : public FLAlertLayer {
protected:
    TextInput* m_tpsInput;
    TextInput* m_fpsInput;

    bool init() override {
        // Inicializamos la capa base sin fondo por defecto para usar tu textura morada
        if (!FLAlertLayer::init(nullptr, "", "", "Guardar", nullptr, 400.0f, false, 300.0f, 1.0f)) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto center = CCPoint(winSize.width / 2, winSize.height / 2);

        // 1. FONDO PERSONALIZADO (Tu ventana morada)
        auto bg = CCSprite::create("menu_purple.png"_spr);
        bg->setPosition(center);
        bg->setScale(0.45f); 
        m_mainLayer->addChild(bg, -1);

        // 2. TEXTO CON TU FUENTE CUSTOM
        auto tpsLabel = CCLabelBMFont::create("Tps", "gothic-font.fnt");
        tpsLabel->setPosition({center.x - 100, center.y + 50});
        tpsLabel->setScale(0.8f);
        m_mainLayer->addChild(tpsLabel);

        auto fpsLabel = CCLabelBMFont::create("Fps", "gothic-font.fnt");
        fpsLabel->setPosition({center.x - 100, center.y - 10});
        fpsLabel->setScale(0.8f);
        m_mainLayer->addChild(fpsLabel);

        // 3. CASILLAS DE ENTRADA CON TU TEXTURA AZUL (box_values.png)
        // Casilla TPS
        auto tpsBg = CCSprite::create("box_values.png"_spr);
        tpsBg->setScale(0.18f); 
        tpsBg->setPosition({center.x + 50, center.y + 50});
        m_mainLayer->addChild(tpsBg);

        m_tpsInput = TextInput::create(100.0f, "TPS");
        m_tpsInput->setPosition({center.x + 50, center.y + 50});
        m_tpsInput->setString(std::to_string((int)g_targetTPS));
        m_tpsInput->getBorders()->setVisible(false); // Quita la caja gris fea de fondo de Geode
        m_mainLayer->addChild(m_tpsInput);

        // Casilla FPS
        auto fpsBg = CCSprite::create("box_values.png"_spr);
        fpsBg->setScale(0.18f);
        fpsBg->setPosition({center.x + 50, center.y - 10});
        m_mainLayer->addChild(fpsBg);

        m_fpsInput = TextInput::create(100.0f, "FPS");
        m_fpsInput->setPosition({center.x + 50, center.y - 10});
        m_fpsInput->setString(std::to_string((int)g_targetFPS));
        m_fpsInput->getBorders()->setVisible(false); // Quita la caja gris fea de fondo de Geode
        m_mainLayer->addChild(m_fpsInput);

        // Acomodar el botón "Guardar" abajo para que no rompa tu diseño
        if (auto buttonMenu = m_mainLayer->getChildByType<CCMenu>(0)) {
            buttonMenu->setPosition({center.x, center.y - 100});
        }

        // 4. BLINDAJE ANTI-CLICS FANTASMAS (Evita que el juego reciba toques de fondo)
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
    }

    // Registrar el menú con máxima prioridad de clics al entrar a escena
    void registerWithTouchDispatcher() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
    }

    void keyBackClicked() override {
        onBtn1(nullptr);
    }

    void onBtn1(CCObject* sender) {
        try {
            int tps = std::stoi(m_tpsInput->getString());
            int fps = std::stoi(m_fpsInput->getString());
            
            g_targetTPS = (float)tps;
            g_targetFPS = (float)fps;

            Mod::get()->setSavedValue<int>("tps", tps);
            Mod::get()->setSavedValue<int>("fps", fps);
            
            Notification::create("Configuración guardada", NotificationIcon::Success)->show();
        } catch (...) {
            Notification::create("Error en valores", NotificationIcon::Error)->show();
        }
        this->removeFromParent();
    }

public:
    static MyOptimizationMenu* create() {
        auto ret = new MyOptimizationMenu();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        float tps = g_syncEnabled ? g_targetFPS : g_targetTPS;
        PlayLayer::update(1.0f / tps);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void onMyMenuButton(CCObject*) { 
        MyOptimizationMenu::create()->show(); 
    }

    void customSetup() {
        PauseLayer::customSetup();
        
        g_targetTPS = (float)Mod::get()->getSavedValue<int>("tps", 60);
        g_targetFPS = (float)Mod::get()->getSavedValue<int>("fps", 60);

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        this->addChild(menu, 100);

        // 5. TU BOTÓN PERSONALIZADO (Ubicación exacta de tu mockup)
        auto spr = CCSprite::create("buttom_open.png"_spr);
        spr->setScale(0.28f); 

        auto btn = CCMenuItemSpriteExtra::create(
            spr, 
            this, 
            menu_selector(MyPauseLayer::onMyMenuButton)
        );
        
        // Esquina superior izquierda, perfectamente debajo del Death Tracker
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        btn->setPosition({35.0f, winSize.height - 75.0f});
        
        menu->addChild(btn);
    }
};

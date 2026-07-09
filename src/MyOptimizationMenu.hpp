#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

// Heredamos de geode::Popup<> de forma limpia sin pasarle argumentos pesados
class MyOptimizationMenu : public geode::Popup<> {
protected:
    bool setup() override;

    // Variables para las cajas de texto de la interfaz
    geode::TextInput* m_tpsInput;
    geode::TextInput* m_fpsInput;

public:
    static MyOptimizationMenu* create();
    void onSave(cocos2d::CCObject* sender);
};

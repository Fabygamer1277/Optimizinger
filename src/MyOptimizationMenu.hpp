#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MyOptimizationMenu : public FLAlertLayer, public TextInputDelegate {
protected:
    // Punteros para leer el texto que ingrese el usuario más tarde
    CCTextInputNode* m_tpsInput;
    CCTextInputNode* m_fpsInput;

public:
    static MyOptimizationMenu* create();
    bool init() override;
    
    // Delegado obligatorio para capturar y validar lo que se escribe en tiempo real
    void textChanged(CCTextInputNode* input) override;

    // Callbacks de los botones
    void onClose(cocos2d::CCObject* sender);
};

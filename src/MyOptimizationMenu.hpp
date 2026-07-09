#pragma once
#include <Geode/Geode.hpp>

// Heredamos de Geode::Popup para obtener estabilidad visual y el botón X nativo
class MyOptimizationMenu : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override;

public:
    // Macro de creación segura de Geode
    static MyOptimizationMenu* create();
    
    // Callbacks para guardar los valores cuando se interactúe
    void onSaveTPS(cocos2d::CCObject* sender);
    void onSaveFPS(cocos2d::CCObject* sender);
};

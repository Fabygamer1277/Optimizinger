#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

// Usamos el espacio de nombres de geode explícitamente para evitar fallos de plantilla
class MyOptimizationMenu : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override;

public:
    static MyOptimizationMenu* create(std::string const& value);
    void onClose(cocos2d::CCObject* sender);
};

#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

class MyOptimizationMenu : public geode::Popup<> {
protected:
    bool setup() override; // Geode espera setup() sin argumentos si la plantilla es vacía
    std::string m_value;

public:
    static MyOptimizationMenu* create(std::string const& value);
    void onClose(cocos2d::CCObject* sender);
};

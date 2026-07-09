#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp> // <- ¡Esta era la que faltaba!

class MyOptimizationMenu : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override;

public:
    static MyOptimizationMenu* create(std::string const& value);
    void onClose(cocos2d::CCObject* sender);
};

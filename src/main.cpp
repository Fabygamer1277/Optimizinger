#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

MyOptimizationMenu* MyOptimizationMenu::create(std::string const& title) {
    auto ret = new MyOptimizationMenu();
    if (ret && ret->initAnchored(240.f, 160.f, title)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyOptimizationMenu::setup(std::string const& title) {
    this->setTitle(title);
    
    // Aquí puedes meter tus opciones de optimización más adelante
    return true;
}

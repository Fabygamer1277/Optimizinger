#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MyOptimizationMenu : public geode::Popup<std::string const&> {
protected:
    // El método setup ahora recibe el string configurado en el template de la clase base
    bool setup(std::string const& title) override;

public:
    // El método create ahora acepta el string que le pasaremos desde el Hook
    static MyOptimizationMenu* create(std::string const& title);
};

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp> // <- ¡ESTA ES LA LÍNEA CRUCIAL QUE FALTA!

class MyOptimizationMenu : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override;

public:
    static MyOptimizationMenu* create(std::string const& value);
};

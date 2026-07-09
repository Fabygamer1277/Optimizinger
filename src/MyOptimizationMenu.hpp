#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class MyOptimizationMenu : public geode::Popup<std::string> {
protected:
    bool setup(std::string value) override;

public:
    static MyOptimizationMenu* create(std::string const& value);
};

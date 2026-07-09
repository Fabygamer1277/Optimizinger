#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MyOptimizationMenu : public FLAlertLayer {
public:
    static MyOptimizationMenu* create(std::string const& title);
    bool setup(std::string const& title) override;
};

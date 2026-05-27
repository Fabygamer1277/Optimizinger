#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

namespace EmirGui {
    // Dokümandaki en güvenli buton oluşturma yöntemi
    inline CCMenuItemSpriteExtra* createCircleBtn(const char* sprName, CCObject* target, SEL_MenuHandler callback) {
        auto spr = CircleButtonSprite::createWithSpriteFrameName(sprName);
        return CCMenuItemSpriteExtra::create(spr, target, callback);
    }

    inline CCMenuItemSpriteExtra* createTextBtn(const char* text, CCObject* target, SEL_MenuHandler callback) {
        auto spr = ButtonSprite::create(text);
        return CCMenuItemSpriteExtra::create(spr, target, callback);
    }
}


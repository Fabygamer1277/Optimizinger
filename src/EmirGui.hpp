#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

namespace EmirGui {

    // Daire sprite buton
    inline CCMenuItemSpriteExtra* createCircleBtn(
        const char* sprName,
        CCObject* target,
        SEL_MenuHandler callback
    ) {
        auto spr = CircleButtonSprite::createWithSpriteFrameName(sprName);
        return CCMenuItemSpriteExtra::create(spr, target, callback);
    }

    // Yazı buton
    inline CCMenuItemSpriteExtra* createTextBtn(
        const char* text,
        CCObject* target,
        SEL_MenuHandler callback
    ) {
        auto spr = ButtonSprite::create(text);
        return CCMenuItemSpriteExtra::create(spr, target, callback);
    }

    // Toggle (açık/kapalı) buton — aktifken sarı, kapalıyken gri görünür
    inline CCMenuItemToggler* createToggleBtn(
        const char* labelOn,
        const char* labelOff,
        CCObject* target,
        SEL_MenuHandler callback
    ) {
        auto sprOn  = ButtonSprite::create(labelOn,  "goldFont.fnt");
        auto sprOff = ButtonSprite::create(labelOff, "bigFont.fnt");
        return CCMenuItemToggler::create(sprOff, sprOn, target, callback);
    }

} // namespace EmirGui

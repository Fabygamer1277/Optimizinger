#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

namespace EmirGui {

    inline CCMenuItemToggler* createToggleBtn(
        const char* onText,
        const char* offText,
        CCObject* target,
        SEL_MenuHandler callback
    ) {
        auto onSpr = ButtonSprite::create(
            onText,
            "goldFont.fnt",
            "GJ_button_02.png",
            0.7f
        );

        auto offSpr = ButtonSprite::create(
            offText,
            "goldFont.fnt",
            "GJ_button_06.png",
            0.7f
        );

        onSpr->setScale(0.7f);
        offSpr->setScale(0.7f);

        return CCMenuItemToggler::create(
            offSpr,
            onSpr,
            target,
            callback
        );
    }
}

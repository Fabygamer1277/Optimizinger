#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "EmirGui.hpp" // Kütüphanemizi çağırdık

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // Ana menüdeki alt menüyü bulalım
        auto menu = this->getChildByID("bottom-menu");

        // Kütüphanemizden butonu oluşturalım
        auto btn = EmirGui::createCircleBtn(
            "GJ_likeBtn_001.png", 
            this, 
            menu_selector(MyMenuLayer::onEmirClick)
        );

        // Geode özelliklerini tanımlayalım
        btn->setID("emir-buton"_spr);
        menu->addChild(btn);
        
        // Menüyü otomatik hizala (Uğraşmanı engeller)
        menu->updateLayout();

        return true;
    }

    void onEmirClick(CCObject* sender) {
        FLAlertLayer::create(
            "Başarılı!", 
            "EmirGui kütüphanesi ve yeni dosya yapısı çalışıyor.", 
            "Tamam"
        )->show();
    }
};

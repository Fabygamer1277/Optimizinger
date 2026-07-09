#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "MyOptimizationMenu.hpp"

using namespace geode::prelude;

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // Aquí va tu lógica para crear el botón del mod en la interfaz principal.
        // Ejemplo de cómo se llamaría a tu menú al presionar el botón correspondiente:
        
        return true;
    }

    // Esta es la función o callback que se ejecuta al presionar tu botón personalizado
    void onMyOptimizationMenuClick(CCObject* sender) {
        // Ahora create() acepta el string correctamente sin generar errores de compilación
        auto layer = MyOptimizationMenu::create("Datos de optimización");
        if (layer) {
            layer->show();
        }
    }
};

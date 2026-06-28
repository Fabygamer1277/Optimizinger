#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);
        
        // Triggers your custom audio asset file safely inside the engine
        FMODAudioEngine::sharedEngine()->playEffect("explode_11.ogg"_spr);
    }
};

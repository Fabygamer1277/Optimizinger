# Emir Hub

Emir Hub is an in-level Geode playtest menu for Geometry Dash. It is built as a compact, Mega-Hack-inspired utility panel focused on quick testing, visual debugging, and safe assist toggles.

<img src="logo.png" width="150" alt="Emir Hub logo" />

## Features

### Player tab
- **No Death**: toggles `PlayLayer::toggleIgnoreDamage` for safe route testing.
- **Practice**: toggles practice mode while keeping other menu settings active.
- **Hide P1**: hides/shows the player icon for visibility checks.
- **Attempts / Progress / Info**: quick UI visibility controls.

### Assist tab
- **Auto Play**: enables the detailed assist engine.
- **Avoid Solid**: predicts lethal contact with `canCollide`/solid objects and jumps before side/head hits while still allowing safe floor landings.
- **Avoid Spike**: treats spike/saw/hazard objects as lethal from every side and routes/taps away from them.
- **Platform**: optional right-move hold for platformer-style tests.
- **Auto All**: enables detailed Auto Play with both solid and spike avoidance.
- **Release**: force-releases any auto-held inputs.

### Visual tab
- **Hitboxes**: uses Geode debug draw safely via `shouldDebugDraw` + `toggleDebugDraw`.
- **Ground / MG / BG FX**: quick visibility toggles for level layers and effects.
- **Debug**: refreshes debug draw settings.
- **Glitter**: refreshes glitter visibility.

### Utility tab
- **Speed**: cycles playtest time-mod values up to 2.00x.
- **Slower / Normal**: quick speed adjustment buttons.
- **Restart**: restarts from the beginning and reapplies active options.
- **Clear CP**: removes all practice checkpoints.
- **About**: in-game feature summary.

## Notes

Auto Play is a heuristic playtest assist, not a perfect verifier. It predicts the player's near-future hitbox against visible solid, spike, saw, and hazard objects so it can avoid lethal side/head/touch collisions while keeping normal floor contact safe. It supports cube, ship, ball, UFO, wave, robot, spider, swing, and platformer-style tests.

## Build instructions

```sh
# Requires the Geode CLI / SDK environment
geode build
```

## Resources

- [Geode SDK Documentation](https://docs.geode-sdk.org/)
- [Geode SDK Source Code](https://github.com/geode-sdk/geode/)
- [Geode CLI](https://github.com/geode-sdk/cli)

# Simple Widget Framework Overview

## Summary
- Work item: Rebuild the widget layer with a small local-player HUD framework.
- Status: Completed
- Started: 2026-03-28
- Last Updated: 2026-03-28

## Scope
- Reintroduce a minimal `Widget` source folder with `NCUserWidget`, `NCUISubsystem`, `NCPlayerHUDWidget`, and shared HUD/widget types.
- Restore `UMG` dependency required for `UUserWidget`-based types.
- Auto-spawn a HUD widget from the local player controller through the local-player UI subsystem.
- Push only simple reticle visibility and focus state from player interaction code into the HUD.
- Leave menu widgets, complaint/report widgets, and generic widget-source/listener plumbing out of scope.

## Completed
- Recreated the core widget type definitions for input policy and HUD state.
- Reintroduced a lightweight `NCUserWidget` base with only shared subsystem access and input policy.
- Added a local-player `NCUISubsystem` that owns HUD lifetime and cached HUD state.
- Added `NCPlayerHUDWidget` with `BindWidgetOptional` caching support for a reticle image.
- Reconnected `NCPlayerControllerBase` so local begin play spawns the configured HUD class through the subsystem.
- Reconnected `NCPlayerCharacter` so door/prop targeting updates HUD reticle focus state.
- Verified the new native widget layer compiles against the project's configured UE 5.7 environment.

## Remaining
- Open the editor and resave `WBP_NCPlayerHUD` if the asset needs designer-side reticle binding cleanup.

## Risks
- Existing widget Blueprint bindings may still expect old fields that were intentionally not restored.
- If the HUD Blueprint uses a different reticle widget name than `ReticleImage`, the optional cache will stay null and visual updates will rely only on exposed state variables.

## Validation
- UE 5.7 editor build: succeeded.
- `rg -a` asset string scan: `WBP_NCPlayerHUD` still references `NCPlayerHUDWidget`, `NCUserWidget`, `bShowReticle`, and `bHasReticleFocus`.

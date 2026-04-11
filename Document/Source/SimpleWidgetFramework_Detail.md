# Simple Widget Framework Detail

## Intent
- The previous widget layer mixed three responsibilities: widget lifetime management, data context injection, and source-listener invalidation.
- The new implementation deliberately keeps only two responsibilities:
  - local-player HUD widget lifetime management
  - simple HUD state storage and push updates
- This keeps gameplay code away from direct widget mutation without rebuilding a generic UI framework too early.

## Implemented Structure
- `Source/NightCaretaker/Widget/NCWidgetTypes.h`
  - Defines `ENCWidgetInputPolicy`.
  - Defines `FNCHUDState`, the small data container used between gameplay code and the UI subsystem.
  - `FNCHUDState` currently stores:
    - `bShowReticle`
    - `bHasReticleFocus`
  - Equality operators are implemented so the subsystem can skip redundant widget updates.
- `Source/NightCaretaker/Widget/NCUserWidget.*`
  - `UNCUserWidget` stays intentionally thin.
  - It exposes:
    - `GetNCUISubsystem()`
    - `GetInputPolicy()`
  - It no longer owns any widget context, source object, listener registration, refresh queue, or source refresh hooks.
- `Source/NightCaretaker/Widget/NCUISubsystem.*`
  - `UNCUISubsystem` is a `ULocalPlayerSubsystem`, which makes it the correct lifetime owner for local-only runtime UI.
  - Stored members:
    - `PlayerHUDWidget`
      - The single active runtime HUD widget instance.
    - `HUDState`
      - The latest simple HUD state, cached whether or not the widget exists yet.
  - Main functions:
    - `ShowPlayerHUD`
      - Creates the HUD widget for the owning local player controller.
      - Reuses the existing HUD instance when the requested class is still compatible.
      - Reapplies cached `HUDState` immediately after creation.
    - `HidePlayerHUD`
      - Removes the HUD from viewport and clears the cached instance pointer.
      - Cached `HUDState` remains so the next HUD instance starts from the latest gameplay state.
    - `SetHUDState`
      - Stores the latest state.
      - Applies it to the active HUD widget when the value actually changed.
    - `SetReticleVisible`
      - Convenience wrapper that only changes `HUDState.bShowReticle`.
    - `SetReticleFocus`
      - Convenience wrapper that only changes `HUDState.bHasReticleFocus`.
    - `ApplyHUDStateToWidget`
      - The internal bridge function that calls `NCPlayerHUDWidget::ApplyHUDState`.
- `Source/NightCaretaker/Widget/NCPlayerHUDWidget.*`
  - `UNCPlayerHUDWidget` is the native parent intended for `WBP_NCPlayerHUD`.
  - Stored members:
    - `bShowReticle`
      - Blueprint-visible runtime flag used by existing HUD logic.
    - `bHasReticleFocus`
      - Blueprint-visible runtime flag used by existing HUD logic.
    - `ReticleImage`
      - `BindWidgetOptional` cache for a named `UImage` in the widget tree.
      - It is optional so missing widget names do not break the widget at runtime.
    - `DefaultReticleTint`
      - Applied when focus is false.
    - `FocusReticleTint`
      - Applied when focus is true.
  - Main functions:
    - `ApplyHUDState`
      - Copies the subsystem state into Blueprint-visible transient members.
      - Calls `RefreshReticlePresentation`.
    - `NativeConstruct`
      - Reapplies the current reticle presentation once the widget tree is guaranteed to exist.
    - `RefreshReticlePresentation`
      - If `ReticleImage` is bound, toggles visibility and tint directly.
      - If `ReticleImage` is not bound, it safely does nothing and leaves Blueprint-side usage of the transient state variables intact.

## Runtime Connection Flow
- `Source/NightCaretaker/System/NCPlayerControllerBase.*`
  - Reintroduced `PlayerHUDWidgetClass`.
  - Added `BeginPlay`.
  - Added `ShowRuntimeHUD`.
  - Flow:
    - `BeginPlay` runs.
    - If the controller is local and `PlayerHUDWidgetClass` is configured, it asks the local player for `UNCUISubsystem`.
    - `UNCUISubsystem::ShowPlayerHUD` creates or reuses the HUD and adds it to the viewport.
- `Source/NightCaretaker/Character/NCPlayerCharacter.*`
  - Added `RefreshHUDState`.
  - This function computes reticle focus from current interaction state instead of mutating widget objects directly.
  - The focus becomes `true` when one of the following is true:
    - the camera is tracing a door target
    - the prop interactor has a preview grab target
    - a door is actively being grabbed
    - a physics prop is currently being held
  - Call sites:
    - `Tick`
      - Keeps the reticle focus synchronized frame-to-frame while aiming.
    - `PostInitializeComponents`
      - Pushes an initial state as soon as the character components are ready.
    - `PawnClientRestart`
      - Re-pushes HUD state after local possession/input restart.
    - `BeginGrabHold`
      - Immediately updates the HUD when a grab interaction starts.
    - `EndGrabHold`
      - Immediately updates the HUD when the interaction stops.

## Why This Is Simpler
- No generic widget context object is injected into every widget.
- No source interface is required on gameplay systems.
- No listener array exists on gameplay-side objects.
- No widget refresh broadcasting occurs across unrelated systems.
- Gameplay code only writes compact state into one local-player subsystem.
- The subsystem owns the only direct reference to the runtime HUD widget.

## Verification Result
- UnrealHeaderTool and C++ compilation succeeded with `UE_5.7` using the project-configured engine association.
- The first build attempt against `UE_5.5` failed before compilation because the project target files require `BuildSettingsVersion.V6` and `EngineIncludeOrderVersion.Unreal5_7`; this was an environment/version mismatch, not a widget-code failure.
- `WBP_NCPlayerHUD.uasset` string inspection still shows:
  - native parent class `NCPlayerHUDWidget`
  - base class chain containing `NCUserWidget`
  - Blueprint variables `bShowReticle` and `bHasReticleFocus`
- This means the recreated native classes and the existing HUD Blueprint still line up on the identifiers that matter for the minimal HUD rebuild.

## Follow-Up
- If future HUD data grows, extend `FNCHUDState` first instead of adding another source/listener layer.
- If an interactive menu is added later, add menu-specific lifetime APIs to `UNCUISubsystem` without changing the HUD path.
- If UI needs callbacks back into gameplay, prefer explicit subsystem functions or delegates over a generic message bus until actual reuse appears.

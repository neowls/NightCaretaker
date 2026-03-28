# Widget Source Removal Detail

## Intent
- The current task is a hard reset of the C++ widget layer so a new structure can be designed from a clean baseline.
- This removal targets code in `Source`, not a redesign yet.

## Removed Source Area
- Deleted all files that previously lived under `Source/NightCaretaker/Widget`.

## External Dependency Cleanup
- `Source/NightCaretaker/System/NCPlayerControllerBase.*`
  - Removed HUD widget class ownership.
  - Removed HUD widget source object ownership.
  - Removed runtime HUD spawn path from `BeginPlay`.
- `Source/NightCaretaker/Character/NCPlayerCharacter.*`
  - Removed HUD widget source include and helper access.
  - Removed reticle/HUD-targeting update function and associated opacity properties.
  - Kept door/prop interaction flow and sprint blocking intact.
- `Source/NightCaretaker/System/Complaint/NCComplaintRuntimeSubsystem.*`
  - Removed `INCWidgetSource` inheritance.
  - Removed widget listener registration, unregistration, and refresh broadcast plumbing.
  - Kept complaint progression and achievement submission flow intact.
- `Source/NightCaretaker/System/Shift/NCShiftStateComponent.*`
  - Removed `INCWidgetSource` inheritance.
  - Removed widget listener storage and refresh broadcasts.
  - Kept shift/chapter/focused-complaint/progression-tag state management intact.
- `Source/NightCaretaker/NightCaretaker.Build.cs`
  - Removed `UMG` from public module dependencies because no remaining source file requires the old widget layer.

## Verification Result
- No remaining references were found in `Source` for:
  - `NCUISubsystem`
  - `NCUserWidget`
  - `NCWidget*`
  - `FNCWidgetContext`
  - `INCWidgetSource`
  - `NCPlayerHUDWidget*`
- The widget source directory itself no longer exists.

## Follow-Up Impact
- Any Blueprint or content-side references to deleted widget C++ classes now need replacement.
- A new UI architecture can be rebuilt without carrying the old listener/context/subsystem design.
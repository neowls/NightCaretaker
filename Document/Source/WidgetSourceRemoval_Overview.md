# Widget Source Removal Overview

## Summary
- Work item: Remove the current widget framework source code and detach all C++ dependencies on it.
- Status: Completed
- Started: 2026-03-28
- Last Updated: 2026-03-28

## Scope
- Delete all source files under `Source/NightCaretaker/Widget`.
- Remove widget-related references from gameplay/runtime source files.
- Remove widget-only module dependencies from the game module if no longer needed.

## Completed
- Deleted the entire `Source/NightCaretaker/Widget` source folder.
- Removed controller-owned HUD widget creation and HUD widget source ownership from `NCPlayerControllerBase`.
- Removed player-character HUD targeting and reticle-source updates from `NCPlayerCharacter`.
- Removed widget-listener interfaces and refresh plumbing from `NCComplaintRuntimeSubsystem` and `NCShiftStateComponent`.
- Removed the runtime `UMG` dependency from `NightCaretaker.Build.cs`.
- Verified that widget-framework source references no longer appear in `Source`.

## Remaining
- Rebuild and resave any Blueprint assets that referenced deleted widget C++ classes.
- Design and implement the replacement widget structure.

## Risks
- Existing Blueprint assets may fail to load or compile until their deleted parent classes/references are replaced.
- Runtime HUD and menu behavior are intentionally absent until a new UI structure is introduced.

## Validation
- `rg` scan for widget framework types in `Source`: no matches.
- `Source/NightCaretaker/Widget` directory presence check: removed.
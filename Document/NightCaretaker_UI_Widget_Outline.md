# NightCaretaker UI Widget Outline

## Purpose

This note summarizes the UI widgets that are currently needed based on `NightCaretaker_307_GDD.md`, `NightCaretaker_307_DevelopmentPlan.md`, and the current runtime framework guide.
It separates what is already prepared as a native C++ base from what still needs authored Blueprint layout or data asset input.

## Prepared Native Widget Bases

- `UNCPlayerHUDWidget`
  - Covers center reticle, interact prompt, tool label, and subtitle overlay.
  - Uses `UNCPlayerHUDWidgetSource` and is auto-shown from `ANCPlayerControllerBase` when a local player starts.
- `UNCComplaintBoardWidget`
  - Intended for the management office complaint board.
  - Supports selecting a complaint id and calling `AcceptComplaint` through `UNCComplaintRuntimeSubsystem`.
- `UNCReportWidget`
  - Intended for the report/result screen after investigation.
  - Supports setting a complaint id, choosing a report result, and calling `SubmitReport` / `CloseComplaint`.
- `UNCMainMenuWidget`
  - Main menu shell for start game, options, and quit flow.
- `UNCOptionsMenuWidget`
  - Options shell for apply/reset/close flow.
- `UNCPauseMenuWidget`
  - Runtime pause overlay shell with resume, options, and return-to-menu hooks.
- `UNCUISubsystem`
  - Local-player widget manager for show/hide/input mode handling.

## Doc-Derived Widget Set

### P0 Runtime UI

- Player HUD
  - Reticle at screen center
  - Fade/opacity response when interaction is available
  - Interaction prompt text
  - Current tool label
  - Subtitle overlay
- Complaint Board
  - Active complaint list
  - Complaint summary / location / category presentation
  - Select and accept complaint
- Report Screen
  - Complaint summary
  - Evidence/report context
  - Report result selection
  - Submit and close complaint
- Settings / Options
  - Basic runtime options shell
- Pause Menu
  - Resume
  - Options
  - Return to main menu

### P1 Soon After

- Main Menu
  - Title screen start flow
  - Continue/load placeholder if save flow lands
- Simple transitional overlays
  - Chapter start title card
  - Objective update / report result feedback
- Save/Load desk interaction UI
  - Only after save system is implemented

## Requires Blueprint / Data Authoring

- Complaint board entries, localized row content, and world location labels
- Report result labels and per-complaint allowed result lists from complaint data assets
- Actual menu button layout, focus visuals, and animation
- Subtitle style, timing rules, and speaker presentation
- Tool icon/text presentation

## Deferred For Later

- Tutorial popups
- Journal/logbook history viewer
- Full save/load menu
- Accessibility/settings persistence backend
- Map or floor navigation overlay

## Suggested First Blueprint Pass

1. Create a BP subclass for `UNCPlayerHUDWidget` and bind reticle, prompt, and subtitle visuals.
2. Create BP subclasses for `UNCComplaintBoardWidget` and `UNCReportWidget` and wire button events to the exposed C++ methods.
3. Create simple shells for `UNCMainMenuWidget`, `UNCOptionsMenuWidget`, and `UNCPauseMenuWidget`.
4. Assign widget classes on the `UNCUISubsystem` owner setup used by the local player.

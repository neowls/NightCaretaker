# Planning Document Refinement Detail

## Intent

The current planning set already establishes the game's tone, platform, scope, and system pillars. The main weakness is not lack of ideas, but lack of direct player-facing explanation. Important information exists, but it is spread across multiple sections. A reader can understand the mood of the game while still not being able to quickly answer what the player actually does each minute, what must be reported, what counts as progress, and why each chapter matters.

This task refined the planning docs with three documentation goals:

1. Make the gameplay purpose legible from a player perspective.
2. Translate atmosphere-heavy planning into actionable content structures.
3. Connect narrative escalation to repeatable production units.

## Documents In Scope

- `Document/NightCaretaker_307_GDD.md`
- `Document/NightCaretaker_307_DevelopmentPlan.md`
- `Document/NightCaretaker_Gameplay_Content_Guide.md`

## Implemented Changes

### GDD Changes

The GDD was updated to make player purpose visible near the top of the gameplay structure.

Added or expanded:

- player role and work principles,
- player action boundaries,
- required progression behavior,
- short/mid/long-term objective framing,
- reward framing,
- chapter-purpose summaries,
- complaint categories and gameplay purpose,
- anomaly categories and escalation logic,
- demo communication goals.

This keeps the full GDD as the master vision document while making it easier to read from a player-experience perspective.

### Development Plan Changes

The development plan was updated to convert abstract gameplay intent into production units.

Added or expanded:

- player objective hierarchy,
- complaint authoring requirements,
- chapter-by-chapter content obligations,
- content package definitions for complaints, anomalies, discoveries, and world-state changes,
- stronger playtest questions about clarity of player purpose.

This makes the document more useful as a scope-control and production reference.

### New Guide

A new guide was added to provide the fastest onboarding path for the concept.

`NightCaretaker_Gameplay_Content_Guide.md` focuses on:

- what the game is,
- what the player does,
- what the player must do,
- what they should not expect,
- why the player keeps moving forward,
- how content is organized,
- what each chapter must communicate,
- what the demo must prove.

## Document Role Separation

- `NightCaretaker_307_GDD.md`: full vision and comprehensive design reference.
- `NightCaretaker_307_DevelopmentPlan.md`: execution, scope, and production guidance.
- `NightCaretaker_Gameplay_Content_Guide.md`: fast communication document for gameplay intent and content structure.

## Validation Outcome

After the edits, a new reader should be able to answer these questions much faster than before:

- Who is the player in this game?
- What actions are actually available?
- What must be done to progress?
- How are complaints and anomalies supposed to function?
- Why does the player keep going back into danger?
- What does each chapter need to achieve?

## Notes

The `apply_patch` tool was unavailable in this session due to a sandbox setup error, so the documentation files were written through PowerShell file writes instead.

## Update Log

- 2026-03-29: Initial analysis and change plan recorded.
- 2026-03-29: GDD and development plan revised.
- 2026-03-29: New gameplay/content guide added.
- 2026-03-29: Work item closed after verification pass.


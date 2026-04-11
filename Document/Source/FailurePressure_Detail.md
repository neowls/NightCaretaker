# Failure Pressure Detail

## Intent

Failure in this project should not be designed as a generic punishment system. The desired emotional result is not irritation, but a sense that the building has become less trustworthy, less stable, and more hostile after the player's mistake or delay.

## Design Problem

The prior documents already said that failure should mainly be "work-state deterioration" rather than death. However, that statement alone leaves an implementation risk: if work-state deterioration only means longer walking, repeated tasks, or broad inconvenience, the result stops being horror and becomes mechanical annoyance.

## Design Answer

Failure must satisfy three conditions:

1. It must make the situation scarier.
2. It must change the next few minutes of play.
3. It must not simply force the same task repetition.

This means failure is best expressed through state corruption and atmosphere escalation rather than raw time loss.

## Failure Categories

### Soft Failure

Used for small delays, missed clues, or minor misreports.

Desired outputs:

- one complaint worsens,
- lighting degrades,
- a familiar area becomes slightly wrong,
- a follow-up report becomes more unsettling.

### Pressure Failure

Used for repeated errors, blackout neglect, or extended exposure to unstable areas.

Desired outputs:

- record corruption,
- repeated complaint resurfacing,
- soundscape hostility,
- rerouted movement,
- stronger 307 intrusion.

### Hard Failure

Reserved for late-game clear danger states only.

Desired outputs:

- short checkpoint rollback,
- forced retreat,
- brief loss of control.

Not allowed:

- long chapter resets,
- replaying large solved sections unchanged,
- punishment with no new horror information.

## Anti-Frustration Rules

- A single failure should not erase 10 to 15 minutes of progress.
- After failure, the player should get a new readable objective within 1 to 3 minutes.
- Failure must change at least one of: light, sound, route, record, or complaint state.
- The player should understand why they are now in a worse state, but should not be fully certain what comes next.

## Document Impact

### GDD

The GDD now defines failure as a horror-state escalation system and gives examples of how failure should mutate the building.

### Development Plan

The development plan now treats failure as a production rule set, with explicit pressure tiers and anti-frustration constraints.

### Gameplay Guide

The guide now explains failure in direct player-facing language so the concept stays communicable.

## Update Log

- 2026-03-29: Failure pressure clarification added to the planning set.

# Player Watch Component (cut alpha content)

`CPlayerWatchComponent` — hash `E036DFA8` (CRC32)

An early-alpha **wristwatch component** found in the February 2012 Watch_Dogs
entity library, attached to Jason's player prototype. Cut before May 2012 and
absent from retail.

## Fields

| field | type | value | meaning |
|-------|------|-------|---------|
| `arkWatch` | Id64 | — | reference to the watch model/entity |
| `fDialAnimRotation` | float | 6.0 | dial animation rotation |
| `fDialRotationSpeed` | float | 6.0 | degrees/second → one full sweep per real minute |

The fixed `6.0 °/s` rate makes the seconds hand track **real time**
(360° / 60 s), independent of game-time-of-day. Two tick-tock sound events
sit alongside it (CRC32 `0x74CE3` / `0x74CE4`).

## Community context

- **Cell** confirmed the component works in the March 30 2012 build.
- **The Silver**: hours/minutes were likely driven separately by in-game
  time-of-day via vertex animation — the standard Disrupt clock approach
  (the huge in-game world clocks use the same trick).
- Fittingly for a watch that shows real time: WD1's world clock only advances
  while the player is moving.

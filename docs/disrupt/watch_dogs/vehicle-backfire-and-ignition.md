# Vehicle Backfire & Ignition (WD1)

Source: community notes, author not recorded. Values below were tested in game by the original author; they are a loose-file integration guide, still marked WIP there.

Covers three separate tweaks to the same vehicle XMLs: restoring backfire, fixing the backfire sound, and making ignition sound realistic.

Vehicle XMLs involved:

- `Vehicle_Police.xml`
- `Vehicle_Speed.xml`
- `Vehicle_Agile.xml`
- `Vehicle_Offroad.xml`
- `Vehicle_Muscle.xml`
- `Vehicle_Traffic.xml`
- `Vehicle_Bike.xml`

## Vehicle backfire

1. Open the XML(s) and search for `BackfireSettings`.
2. You will find multiple blocks. Replace each one, one by one, with the matching template below.

### Speed, Agile, Traffic, Police

```xml
<BackfireSettings
    fBackfireRpmChangeMinSpeed="1000.0"
    fBackfireRpmChangeSpeedLag="0.25"
    vec2BackfireRpmRange="4000.0,10000.0"
    bBackfireRpmIsUp="0"
    bBackfireRpmIsDown="1"
    bBackfireNonPlayer="0"
    fBackfireFireChance="5.0" />
```

### Muscle, Offroad

```xml
<BackfireSettings
    fBackfireRpmChangeMinSpeed="1000.0"
    fBackfireRpmChangeSpeedLag="0.25"
    vec2BackfireRpmRange="4000.0,10000.0"
    bBackfireRpmIsUp="0"
    bBackfireRpmIsDown="1"
    bBackfireNonPlayer="0"
    fBackfireFireChance="4.0" />
```

The only difference between the two is `fBackfireFireChance` (5.0 vs 4.0).

### Bikes

Bikes rev up to the 20,000 range, so the RPM window has to be widened:

```xml
<BackfireSettings
    fBackfireRpmChangeMinSpeed="1000.0"
    fBackfireRpmChangeSpeedLag="0.25"
    vec2BackfireRpmRange="4000.0,20000.0"
    bBackfireRpmIsUp="0"
    bBackfireRpmIsDown="1"
    bBackfireNonPlayer="0"
    fBackfireFireChance="5.0" />
```

## Exhaust fire sound

Applies to Speed, Bike, Agile, Offroad, Police, and some Muscle cars.

- Some Muscle cars have their own distinct exhaust fire sounds worth keeping.
- Find the `FF` nulled values and replace them with `61 4A`. `FF` means no sound, so the backfire is silent until you do.
- The Speed XML does have its own sound values, but they are very weak, so replace them with `61 4A` as well.

1. Search for `sndExhaustFireSound`.
2. Change its value:

   ```xml
   sndExhaustFireSound="61 4A"
   ```

## Realistic ignition

Every car with hiding support has a distinct ignition and turn-off sound. Syncing the normal ignition to the hiding one makes startup feel more authentic, and raising the engine start time to 2.75 s makes the audio line up with the animation.

> **Important:** if `sndEngineIgnitionFromHiding` or `sndTurnOffEngineWhileHiding` is `FF`, leave that vehicle alone. `FF` means the car does not use the hiding mechanic, so there is nothing to copy.

For each XML:

1. Search for `fEngineStartTime` and replace it with `fEngineStartTime="2.75"`.
2. Search for `sndEngineIgnitionFromHiding` or `sndTurnOffEngineWhileHiding`. You will find this block:

   ```xml
   sndEngineIgnition=""
   sndEngineIgnitionFromHiding=""
   sndTurnOffEngine=""
   sndTurnOffEngineWhileHiding=""
   ```

3. Copy the "FromHiding" values over the normal ones:
   - `sndEngineIgnitionFromHiding` → `sndEngineIgnition`
   - `sndTurnOffEngineWhileHiding` → `sndTurnOffEngine`

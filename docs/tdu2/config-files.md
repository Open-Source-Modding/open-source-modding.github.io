# TDU2 Config Files and Physics
Source: TDU2 modding workspace notes.

`.cpr` files carry the game's config and save data. They are XTEA-encrypted payloads, so editing one is a three-step loop: decrypt with `tdudec`, edit the resulting text, re-encrypt with `tdudec`. `DevicesPC` and `Physics` are handled this way, and `Language.cpr` is listed among the same config overrides.

## tdudec types

| Type | Use |
|------|-----|
| `0` | Savegames |
| `1` | Config |

`.cpr` config files use XTEA type 1 encryption. Pass the type as the last argument to `tdudec`.

## Hard constraints

- **CRLF line endings are required** (`\r\n`) for `.cpr` config source files. LF-only files break the round trip.
- **Trailing binary bytes after the text in config files are required.** Do not strip them.
- No build system. The source file is a raw text file and the tool is run by hand.

## Physics

Physics lives in `Physics.txt`, which is edited as text and then re-encrypted to `.cpr`.

Two things are deliberately not in `Physics.txt`:

- **Tire models live in their own archives.** They are Pacejka tire files, not part of `Physics.txt`. See [Tire models](#tire-models).
- **Per-car friction comes from the game database** (`.xmb`/`.db`), not from `Physics.cpr`.

So retuning grip has two parts: the friction values come from the game DB, and the tire model tables come from `tires.bnk`. `Physics.txt` covers everything else.

## Tire models

TDU2 uses a Pacejka tire model, and the data is split across two BNK archives rather than sitting in `Physics.txt`.

| Archive | Entries | Holds |
|---------|---------|-------|
| `…\Bnk\Physics\tires.bnk` | `tire1.bpjka` … `tire6.bpjka`, 384 bytes each | Sampled slip and force tables plus unnamed curve scalars |
| `…\Bnk\Vehicules\Tires\tires.bnk` | `.xmb` entries named `Tire`, `Tire1`, `Tire2`, `Tire3`, `Tire1_Bike`, `Tire1_Supercar`, `Tire_MS9`, `Tire_2_MS9`, `Back_up_Tire2`, `Offroad_Bad`, `Offroad_Good` | The Pacejka coefficient arrays and optimal slip values |

The `.xmb` entry set varies between copies of the archive: one retail copy lists `Tire1`, `Tire2`, `Tire1_Bike`, `Tire1_Supercar`, `Offroad_Bad` and `Offroad_Good`, while an exported copy carries the wider `Tire`/`Tire3`/`Tire_MS9`/`Tire_2_MS9`/`Back_up_Tire2` set as well. Treat the name list as a version-dependent roster rather than a fixed schema. The `.bpjka` side is stable at six entries.

Both archives are named `tires.bnk` and both live under `Resources\Final\PC\EURO\Bnk\`; the shorter entry names (`.xmb`) carry the coefficients, and the longer ones (`.bpjka`) carry the lookup tables. Within an archive the entries are addressed by an embedded install path (`Eden-Prog\Games\TestDrive2\…`), so a replacement archive has to keep the same prefix and extension fields.

Community beta physics packs ship both files side by side, with the `Vehicules` copy larger than the `Physics` one (6,144 bytes against 4,096 in one circulated pack). Replacing only the `Physics` archive leaves the coefficient arrays untouched, which is one reason table-only edits can appear to do nothing (see the caveat below).

Each `.bpjka` payload begins with the ASCII string `PACEJKA TIRE FLE` (the shipped magic text is missing the `I`). [TDU2 Emploder](input-devices.md) opens the archive by name (`tires.bnk.xmb`) and shows one entry at a time:

| Field | Meaning |
|-------|---------|
| `SlopeFactor C` | Slope of the linear part of the force curve |
| `FishTireMu_z`, `FishTireMu_z|1000` | Peak friction coefficient (the `|1000` field is the same value scaled by 1000) |
| `ScaleFactor` | Multiplier applied to the curve |
| `Unk_10` … `Unk_23` | Unnamed scalars, including two zeroed pairs |
| `SlipTable[0..31]` | 32 slip ratios |
| `ForceTable[0..31]` | 32 normalized forces, one per slip ratio |

`SlipTable` runs from `0.005` to `0.4` in even steps and `ForceTable` carries the matching normalized force, rising to a peak and then falling off toward the high-slip end. For one shipped entry the peak sits at `SlipTable[7] = 0.094196` with `ForceTable[7] = 0.697467`, decaying to `0.49114` by slip `0.4`. Emploder reports this in its status bar: the vertical load it used (`Fzo = 1150 N`), how many slip/force pairs are valid, the peak force and the index it occurs at, the slip at peak, and a ratio between peak force and slip at peak.

On the `.xmb` side, the `PacejkaConfig` object carries the model itself:

| Field | Meaning |
|-------|---------|
| `Name` | Tire name, for example `Tire1`, `Offroad_Good` |
| `LateralCoefficientArray` | Magic Formula coefficients for lateral force |
| `LongitudinalCoefficientArray` | Magic Formula coefficients for longitudinal force |
| `AligningMomentCoefficientArray` | Magic Formula coefficients for self-aligning torque |
| `OptimalSlipRatio` | Slip ratio the model treats as optimal |
| `OptimalSlipAngle` | Slip angle the model treats as optimal |

So the game ships the real coefficient arrays and a sampled version of the same curve side by side. The shape in `ForceTable`, a rise to peak grip around `0.1` slip followed by a long decline, is the shape the coefficient arrays produce; the [Pacejka background](#tire-modelling-background) below explains where it comes from.

To edit a tire model: open the archive in Emploder, select an entry, change the scalars or the table pairs, and save. Because the values are table entries rather than formula inputs, batch-editing all tires at once is a table edit.

Community-reported caveat: on the PP2 Discord, changes to the `.bpjka` tables were described as having very little effect in game unless the values were pushed to extremes, with the suggestion that the game applies its own checks on top. Treat tire-table edits as low-yield until measured in game.

## Tire modelling background

The `PacejkaConfig` arrays are fitted instances of the **Pacejka Magic Formula** (MF), a function `y = f(x)` that predicts the force a tire develops from its slip.

- The input `x` is **slip ratio** (longitudinal) or **slip angle** (lateral). Slip ratio is the difference between the tire's angular velocity and its actual velocity over the ground; a drive wheel turns slightly faster than the road surface beneath it even at steady speed, because the contact patch compresses at the leading edge.
- Three separate functions exist, one each for **longitudinal force**, **lateral force** and **self-aligning torque** (the force felt through the steering wheel). Each has its own coefficient set, which is why the config carries three arrays.
- The coefficients (`b0`-`b12` in the classic Pacejka '94 form) shape the curve. Fitting them to measured data from a real tire is the whole point of the model: once fitted, the tire's behaviour can be predicted without the physical tire. `OptimalSlipRatio` and `OptimalSlipAngle` record where the fitted curves peak.

Read a longitudinal curve as normalized force against slip ratio: it climbs steeply, peaks at a low slip value (around `0.1` in the published curves), then falls away and flattens toward an asymptote. Curves for lower friction surfaces sit lower but keep the same shape, which is what the `ForceTable` in a `.bpjka` entry stores as its 32 samples.

Two caveats matter when reading any Pacejka-derived data:

- **Slip formulas are unstable at low speed.** The ratio is `0/0` at a standstill and numerically noisy as `V` approaches zero, so implementations clamp or special-case low speeds.
- **Slip angle alone has no speed term.** The same slip angle yields the same lateral force whether the car is crawling or at speed, which is why arcade handling and full simulations diverge in how they feed the curves.

Real coefficient sets are scarce and expensive, because they come from instrumented tire testing (the industry route is measurement on a test rig, then model fitting; commercial labs offer this with Pacejka 5.2 plus their own fitting software). Most sets circulating for games are hand-tuned to feel right rather than measured, which is why "more realistic" usually means better fitting practice, not genuine data. The curves themselves matter less than the slip values fed into them. For combining the longitudinal and lateral outputs into a single friction force, the usual reference is Brian Beckman's *The Physics of Racing*, chapters 24 and 25.

Sources: [Facts and myths on the Pacejka curves](https://www.edy.es/dev/2011/12/facts-and-myths-on-the-pacejka-curves/) (Edy's Projects, 2011, updated 2020) and [Tire modelling Pacejka 5.2](https://www.dufournier.com/tire-analysis/tire-modelling-pacejka) (Dufournier, a commercial tire-testing lab).

## Device mapping

`DevicesPC.ini` is the text source that maps USB hardware IDs to XMB input profiles. It is encrypted to `DevicesPC.cpr` for the game, with type 1:

```bash
./tdudec e DevicesPC.ini DevicesPC.cpr 1
```

The line format that ties a USB product/vendor ID to a profile is:

```ini
HIDDefaultConfig = "ProductIDVendorID-0-0-00-504944564944" "Device Name" "DeviceName.xmb"
```

The trailing `504944564944` is the ASCII `PIDVID`. See [Input device profiles](input-devices.md) for the full mapping and the HID field layout.

Because `DevicesPC` is a `.cpr`, its source must keep CRLF too. The same decrypt-edit-encrypt loop applies.

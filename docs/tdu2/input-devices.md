# TDU2 Input Device Profiles
Source: TDU2 modding workspace notes.

TDU2 input profiles are JSON keymaps compiled to binary `.xmb` device profiles. The game reads the XMB; the JSON is the editable source. Profiles exist here for Xbox 360, Fanatec, Razer, and keyboard/mouse.

## JSON structure

Every value is wrapped in a typed object. Do not flatten or remove the wrappers, the game parser requires them.

```json
{
  "_PrimitiveType": "UINT8",
  "Value": 0
}
```

Types in use: `UINT8`, `UINT16`, `UINT32`, `BOOL`, `STRING`.

### Top-level keys

| Key | Meaning |
|-----|---------|
| `FileVersion` | Profile format version. |
| `FFB` | Force feedback. |
| `VibrationsLevel` | Vibration strength. |
| `SteeringLinearity` | Steering response curve. |
| `SpeedFactor` | Speed influence. |
| `SteeringDamping` | Steering damping. |
| `DeadZone` | Centre deadzone applied by the game. |
| `ThrottleLinearity` | Throttle response curve. |
| `BrakeLinearity` | Brake response curve. |
| `ClutchLinearity` | Clutch response curve. |
| `KeyMap[]` | Array of action-to-channel bindings. |

### KeyMap entry fields

| Field | Meaning |
|-------|---------|
| `ActionName` | The action being bound (for example `CAR_ACCEL`). |
| `ChannelName` | The device channel, for example `BUTTON_4`. |
| `ChannelNameAND` | Secondary channel for combined bindings. |
| `ChannelLocName` | French display string (the game's original locale). Changing it has no functional effect. |
| `DevInstanceHID` | Device instance HID data (see below). |
| `DevType` | Device type. |
| `DevSubType` | Device subtype. |
| `DevIndice` | Device index. |
| `Inverted` | Invert the axis. |
| `Combined` | Combined binding flag. |

## Device identification

A device is identified by `DevType` / `DevSubType` plus the `DevInstanceHID` values.

| Device | DevType | DevSubType | DevInstanceHID Data1 | HID4 (bytes 2-7) |
|--------|---------|------------|----------------------|-------------------|
| Xbox 360 pad | 2 | 3 | 2741830387 | `50 49 44 56 49 44 00 00` ("PIDVID") |
| Generic joystick (Fanatec) | 2 | 2 | 26676919 | `50 49 44 56 49 44 00 00` |
| Keyboard/mouse | 0 | 0 | 1864182625 | `BF C7 44 45 53 54 00 00` ("DEST") |

The HID4 bytes decode to ASCII: `50 49 44 56 49 44` is "PIDVID" and `44 45 53 54` is "DEST".

The Fanatec GT DD Pro exposes two HID variants for native versus compatibility mode (Data1: 2100919 versus 4198071). Match the HID to the exact device mode.

## Channel names

| Group | Channels |
|-------|----------|
| Buttons | `BUTTON_0` through `BUTTON_18` (0-indexed) |
| D-Pad | `UP`, `DOWN`, `LEFT`, `RIGHT` |
| POV hat | `POV_0_LEFT`, `POV_0_RIGHT`, and so on |
| Sticks | `STICK_0_UP`, `STICK_0_DOWN`, `STICK_0_LEFT`, `STICK_0_RIGHT`, `STICK_1_*` |
| Sliders | `SLIDER_1_BACKWARD` |
| Unbound | `ChannelName` = `""` with all HID and DevType fields zeroed |

## Common actions

`CAR_ACCEL`, `CAR_BRAKE`, `CAR_CLUTCH`, `CAR_LEFT`, `CAR_RIGHT`, `CAR_HBRAKE`, `CAR_GUP`/`CAR_GDOWN` (gear up/down), `CAR_G1ST`–`CAR_G7TH` (individual gears), `CAR_GREV` (reverse), `CAR_CAMERA`, `CAR_BACKVIEW`, `CAR_LIGHT`, `CAR_HORN`, `CAR_WINDOWS`, `MAP`, `MAP_CURSOR_ZOOMIN`/`ZOOMOUT`, `BACK_ON_TRACK`, `RADIO_VOL_MORE`/`LESS`, `CAR_LEFT_BLINKER`, `CAR_RIGHT_BLINKER`.

## Workflow: XMB → JSON → edit → XMB

1. Original `.xmb` files are binary device profiles shipped with the game.
2. Export to JSON via TDU2Emploder.
3. Edit the JSON profile.
4. Re-import the edited JSON via TDU2Emploder and export as `.xmb`.
5. Place the `.xmb` in the game's `Input Devices` directory.

JSON files in the workspace are the editable exports of the XMB originals. `DeviceX360Pad.xmb` is the binary that corresponds to `xbox360_fixed.json`.

## DevicesPC mapping

The game maps USB hardware IDs to XMB profiles through `DevicesPC.ini`. The line format is:

```ini
HIDDefaultConfig = "ProductIDVendorID-0-0-00-504944564944" "Device Name" "DeviceName.xmb"
```

- `Data1` in the XMB is the hex `ProductIDVendorID` (for example `0x1970EB7` = 26,676,919 for the generic Fanatec joystick).
- `Data4` is the ASCII `\0\0PIDVID` = `[0, 0, 80, 73, 68, 86, 73, 68]`.

Unsupported wheels fall through to `default_kb`, which has **deadzone=5**. This is the root cause of centre-deadzone complaints.

### Adding a new wheel

1. Find the wheel's USB PID/VID (Windows Device Manager → Properties → Hardware IDs).
2. Add a `HIDDefaultConfig` entry to `DevicesPC.ini`.
3. Re-encrypt it:

```bash
./tdudec e DevicesPC.ini DevicesPC.cpr 1
```

4. Export the XMB via TDU2Emploder, edit the JSON profile, and re-import as XMB.
5. Place the XMB in the game's `Input Devices` directory.

### tdudec versus TDU2Emploder

Different tools, different jobs:

- **TDU2Emploder**: XMB ↔ JSON conversion for control profiles.
- **tdudec**: decrypt/encrypt `.cpr` config files such as `DevicesPC` and `Physics`. Uses XTEA type 1 encryption for `.cpr` files.

## Case study: Fanatec GT DD Pro

**USB IDs**: native mode `0EB7:0020` (Data1 = 2100919), compatibility mode `0EB7:0040` (Data1 = 4198071). DevType = 2, DevSubType = 2.

**Button mapping** (from hid-fanatecff `ftec_keymap[]`, `custom_button_mapping=true`):

| HID Button | Physical Button | TDU2 channel |
|------------|-----------------|--------------|
| 0x01 | Square | BUTTON_0 |
| 0x02 | Cross | BUTTON_1 |
| 0x03 | Circle | BUTTON_2 |
| 0x04 | Triangle | BUTTON_3 |
| 0x05 | Right paddle (gear up) | BUTTON_4 |
| 0x06 | Left paddle (gear down) | BUTTON_5 |
| 0x07 | R2 | BUTTON_6 |
| 0x08 | L2 | BUTTON_7 |
| 0x09 | Share/Options | BUTTON_8 |
| 0x0a | Create/Touchpad | BUTTON_9 |
| 0x0b | R3 | BUTTON_10 |
| 0x0c | L3 | BUTTON_11 |

The D-Pad is reported as a POV/hat switch (`POV_0_*`), not as button indices.

**Profile mapping** (sensible defaults, subject to revision):

| Action | Binding |
|--------|---------|
| Gear up | BUTTON_4 (right paddle) |
| Gear down | BUTTON_5 (left paddle) |
| Handbrake | BUTTON_1 (Cross) |
| Camera | BUTTON_3 (Triangle) |
| Back view | BUTTON_2 (Circle) |
| Lights | BUTTON_0 (Square) |
| Horn | BUTTON_8 (Share) |
| Map | BUTTON_9 (Options) |
| Back on track | BUTTON_10 (R3) |
| Reverse | BUTTON_11 (L3) |
| Windows | POV_0_LEFT |
| Radio channel | POV_0_RIGHT |
| Radio volume ± | POV_0_UP / POV_0_DOWN |

Blinkers stay on the keyboard, because the D-Pad is fully allocated. Individual gears (`CAR_G1ST`–`CAR_G6TH`) are unbound; the DD Pro uses sequential paddles.

**Autodetect crash**: a known TDU2 bug. "Detect Device" only works for wheels supported at launch (G25, G27, Porsche 911 Turbo S, and others). The DD Pro was not supported. Fanatec wheels expose multiple HID interfaces (native + compat + Col02), and TDU2 crashes while enumerating them. Workaround: build the profile manually via `DevicesPC.ini` and skip autodetect entirely. If the game freezes on startup with the wheel plugged in, disable the secondary HID device in Windows Device Manager (`HID\VID_0EB7&PID_0004&REV_0474&Col02`).

Sources for this case study: hid-fanatecff (gotzl/hid-fanatecff), Steam discussions, GameFAQs, and TurboDuck.net.

## Case study: Razer Wolverine V2

**USB ID**: `1532:0a29` (XInput, DevType = 2, DevSubType = 3). Same bindings as the Xbox 360 profile, with tuned settings.

**Firmware deadzone bug**: the Wolverine V2 hard-codes a roughly 15-20% deadzone in firmware that cannot be disabled on Linux, because the Razer Controller Setup app is Windows-only. The profile uses `DeadZone=0` so the game deadzone does not stack on top. A companion userspace evdev daemon that rescales the stick axes to compensate is used alongside the profile.

**DevicesPC.ini entry**: `0A291532-0-0-00-504944564944` maps to `DeviceRazerWolverineV2Pad.xmb`.

## Gotchas

- `ChannelLocName` is the French display string from the game's original locale. Changing it has no functional effect.
- `remap_360.py` has a hardcoded input path that points at the author's own machine; update it before running.
- `xbox360_fixed.json` and `RazerWolverineV2.json` share button bindings but differ in tuning settings (`DeadZone`, `BrakeLinearity`, `SteeringLinearity`, `SteeringDamping`, `ThrottleLinearity`).
- Unbound actions use zeroed HID fields (all `0`) with an empty `ChannelName`.
- `remap_360.py` only adds missing D-Pad bindings; it does not touch existing button assignments (idempotent check via `has_controller()`).

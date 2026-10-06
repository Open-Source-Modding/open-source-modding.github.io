# Test Drive Unlimited 2 Modding
Source: TDU2 modding workspace notes.

Test Drive Unlimited 2 modding is file surgery: the game ships plain-ish structures wrapped in a light encryption layer, and nearly every mod is either a config file that gets decrypted, edited, and re-encrypted, or a sound/device bank that gets round-tripped through JSON. There is no build system and no compile step. You edit a source file and run a tool by hand.

## The two file families

| Family | Extensions | What it holds | Round trip |
|--------|-----------|---------------|------------|
| Config and save files | `.cpr` | Settings and savegames, XTEA encrypted | `tdudec` decrypt → edit text → `tdudec` encrypt |
| Structure banks | `.xmb`, `.bnk` | Device profiles, car sound configs, sound banks, gauges | TDU2Emploder export → JSON → edit → import |

A `.cpr` is not JSON. It is an encrypted text payload, so the edit step is a text editor and the only tooling is the encrypt/decrypt pass. An `.xmb` is a binary structure, so the edit step needs a schema-aware converter to turn it into JSON and back.

## Tools

| Tool | Purpose |
|------|---------|
| `tdudec` | Encrypt/decrypt `.cpr` files. Type 0 = savegames, type 1 = config. CRLF is required for config source. |
| TDU2Emploder | XMB ↔ JSON conversion for configs, sound configs, and input profiles. |
| PP2 | Car packs and slot injection. The route for add-on (non-replacement) cars. |

`tdudec` and TDU2Emploder are different tools with different jobs. `tdudec` handles `.cpr` encryption; TDU2Emploder handles XMB/JSON structure conversion. Do not confuse them.

## Hard constraints

- **UI is the practical limit.** Per the PP2 lead developer, the only real limitation in TDU2 modding is UI work (VHF/SWF ActionScript). Audio, physics, and input are all flexible.
- **CRLF line endings are required** (`\r\n`) for `.cpr` config source files.
- **Trailing binary bytes after the text in config files are required.** Do not strip them.
- **No build system.** Sound and input mods are raw files plus manual tool runs.

## Pages

- [Car sound mods](car-sound-mods.md): the WAV directory plus `CarVSTConfig.json`/`.xmb`, the typed wrapper convention, `nWaveIndex` and `nCarID`, physics-to-gain curves, and the engine pitch chain.
- [Input device profiles](input-devices.md): the JSON profile schema, HID device identification, the channel and action vocabulary, `DevicesPC.ini`, and the Fanatec and Razer case studies.
- [Config files and physics](config-files.md): `tdudec` types, XTEA type 1 encryption, the `.cpr` constraints, and where physics and tire data actually live.

## Community

- **PP2** (Project Paradise 2): active modding community and car pack framework.
- **Kuxii**: PP2 lead, mod tool developer; source of the constraints notes.
- **Nikki [MEOW]**: car modder; discovered the SLK55 naming trap.
- **Skies [PP2]**: beta liveries mod, car pack contributor.

Mod releases are posted on turboduck.net and playground.ru.

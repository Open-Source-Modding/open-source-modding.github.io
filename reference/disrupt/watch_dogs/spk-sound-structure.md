# Watch Dogs — SPK Sound Pack Structure & Switch Events

> Community reverse-engineered sound modding knowledge (Discord: Vector4782 [Villain
> Arc] 1/9/18, みる97 3/22/23). SPKs are the audio pack containers that the binary
> `soundidlinelinks.slid` links to via its LinkID column (see
> [SLID Format](slid-format.md)). Here's what is *inside* an SPK.

## SPK file IDs

SPKs are referenced by their hex hash as the filename (`<hash>.spk`), matching the
SLID LinkID (Col B) range and the `<Elem>`/`<Id>`/`<eventRef>` values in their XML
internals. e.g. `0001803e.spk`, `00018029.spk`, `0010d5ae.spk`, `000170a4.spk`.

## Two XML container types

### 1. Parent / child track — `<m_events>` (multi-resource track)

A parent SPK references child SPKs as `<Elem>` elements under `<m_events>`. Example:
the **player gunshot** SPK (`0001803e` is the parent — holds playback properties /
loop behavior):

```xml
<m_events>
    <Elem>00018029</Elem>   <!-- the shot itself (a 4-layer multilayer track) -->
    <Elem>0010d5ae</Elem>   <!-- echo / reverb / location-based reflection — keep as-is -->
    <Elem>000170a4</Elem>   <!-- casing sounds — MUST add this Elem to hear shell casings -->
</m_events>
```

- **Top** = the shot, split into 4 layers (multilayer file).
- **Middle** = location-based reflection/echo (`0010d5ae` — contains many sounds).
- **Last** = casing (`000170a4`).
- To replace a gunshot: create a single-layer SPK swapping just the player shot sound,
  then add casing + echo/reverb as `<Elem>` children in the parent SPK.

**Adding a new layer:** add it as an `<Elem>` in the parent SPK (e.g.
`<Elem>B5A2EE00F</Elem>` for `B5A2E00F.spk`), then register it in **depload** (search
the parent SPK and its path). New SPKs may need depload too.

### 2. Switch event — `<type>SwitchEventDescriptor</type>` / `<m_elements>`

Separates **player** vs **AI** sound per SPK via a switch event. The weapon archetype
declares the switch values:

```xml
sndswvlAISoundSwitchValue="#434D"   <!-- 17229 = AI sound spk id -->
sndswvlPlayerSoundSwitchValue="#434C" <!-- 17228 = player sound spk id -->
```

The switch-event SPK maps each switch value to a sound event:

```xml
<type>SwitchEventDescriptor</type>
<Id>0004eacc</Id>                       <!-- this SPK's id → change to your new spk id -->
<m_elements>
    <Elem><eventRef>0004eac2</eventRef><switchValueId>17228</switchValueId></Elem>  <!-- player -->
    <Elem><eventRef>0004eb0e</eventRef><switchValueId>17229</switchValueId></Elem>  <!-- ai -->
    <Elem><eventRef>0004eb0e</eventRef><switchValueId>1147235</switchValueId></Elem>
    <Elem><eventRef>0004eb0e</eventRef><switchValueId>1211638</switchValueId></Elem>
</m_elements>
```

Rules from the community:
- **`17228`** = player sound spk id, **`17229`** = ai sound spk id. Change these in your
  copy of the switch-event SPK.
- This keeps player audio from spilling into NPC/AI sounds — set a **switchevent**
  inside the SPK for that separation.
- For AI-only, use `eventRef = ffffffff`.
- To drive a moving-part/trigger sound with the switch: point the trigger sound SPK at
  your new switch-event SPK (new SPK may need depload).

## Tools

- **SPK tool** (community, current) — the recommended SPK editor (supersedes the Mirus
  010 script for most cases).
- **Mirus 010 script** — still useful for multilayer in-game tracks (e.g. replacing the
  "Dot Connexion" music track with 2 layers).

## Relation to SLID

`soundidlinelinks.slid` (binary) maps `SoundID → LinkID (.spk hash)`. This doc covers
what those `.spk` files contain. `snd.txt` / `Eng_snd_Fileslist_Loc.csv` resolve
`SoundID → dialogue text`. See [SLID Format](slid-format.md).

## See also

- [SLID Format](slid-format.md)
- [Wii U Debug Symbols](wii-u-debug-symbols.md) — `SND_*` engine symbols backing the
  sound system
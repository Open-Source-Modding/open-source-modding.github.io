# material.bin FORMAT — CONFIRMED (010 Editor template = authoritative spec)

Source: `/home/selene/Documents/Code/re/Disrupt/ConvertMaterials/res/material.bin.bt` (010 Editor v9.0.1 template) + `materialNames.txt` (916 param names = CRC32 dict) + `types.xml` (59KB param type defs). Converter: `ConvertMaterials.exe` (WD1, 2020).

## Structure (LITTLE-endian on PC)
```
Header: 18 x u32: magic('TAM\x00'=0x004D4154), version, unk2..7, size, size2, unk8, unk9, size3, unk10, size4, unk11, unk12
Name:   u32 nameSize, name bytes, pad to 4
ShaderSetting: u32 shaderNameSize, shader bytes, pad-to-4 (minus 2 if mod>=2), then InitSettings:
        u16 unk0, [u8 unk1_1, u8 unk1_2 if pad>=2], u8 unk2, u8 unk3, u32 unk4, i32 unk5, i32 unk6, i32 unk7
Parameters: u8 unk74, u8 unk75, u16 parameterCount, then per param:
        pad to 4, u8 type, skip pad, [u32 nameID if type-1<=10]  <- nameID = CRC32(param name)!
        switch(type):
          1: u32 | 2: vec2 (2x u32) | 3: vec3 | 4: vec4 | 5: i32 | 6: byte(bool)
          7: u32 | 8/9/10: u32 size + string bytes | 11: u32
bUseGradient: u32; if==1: grad_vec i32, vecs[grad_vec] (4x u32), id u32, unk1 u32, unk2 u8
eof: u32 always 0
```

## KEY FINDINGS (verified 2026-08-19)
- **nameID = standard CRC32 of the parameter name** (zlib.crc32). VERIFIED: DiffuseColor=0x9015351F, Opacity=0x8C4054FA, AmbientColor=0xDC05433D, HeightScaleColor=0xAFD746E7, IgnoreMapEffect=0x62E3EAB4, DiffuseTexture0State=0xA94DD2BF. 6/6 confirmed.
- **Enum values also hashed**: "ColorWrap" → 0x13371135 (found in DiffuseTexture0State value slot).
- Binary param ORDER ≠ XML order (binary: Opacity, DiffuseColor, DiffuseTexture0State, IgnoreMapEffect, UseFakeLighting, HeightScaleColor, AmbientColor, then 6x count=4 vec4 blocks = DiffuseTiling-ish 1,1,0,0).
- My earlier "count+hash" reading was wrong — the leading u32 in each block is the **TYPE byte + count u16 area**, not a value count. Parse: u8 type, then nameID, then typed values per table above.
- Sample: `~/Documents/Modding/WDL/unpacked/common/graphics/_materials/amundt-m-9223372119553324321.material.bin` (416B) ↔ `leak/ubisoft/data/graphics/_materials/amundt-m-9223372119553324321.material.xml` — SAME hash name in both games (WDL + leak), XML = editor form of binary.
- 24,079-24,082 .material.xml in leak (`~/Documents/Code/re/Disrupt/leak/ubisoft/data/graphics/_materials/`) = ground truth for every material.

## Community notes (unsorted/MaterialBin/)
- Disrupt Editor material converter (fan-made, WD1-origin, works all 3 WD games) has BUGS: wrong file length on bin→xml→bin (crashes!), float precision loss. Hex edit safest.
- Texture paths in XML use backslash `graphics\...`; textureHistory field lists past texture paths.
- XBT = header + DDS; header controls brightness/usage; keep original header! _high.xbt = highest mip (separate file, referenced in header). Tools: hV_WD1ModdingKit.exe (drag xbt↔dds), Gibbed.Dunia (svn.gib.me/public/dunia2/).
- selMaterialOverridesId = enum from material "Name" binhex→unsigned long (Intel byte order).

## NEXT STEPS
1. Write full decoder from the .bt template → Python, test on all 70 WDL .material.bin ↔ leak XML (verify roundtrip).
2. Dump spec to web repo: `reference/disrupt/watch_dogs_legion/material-bin-format.md` (or watch_dogs/).
3. ES needs: material.bin+XBT full reverse, addon vehicles, custom GUIDs. XBT = user gathering community info.
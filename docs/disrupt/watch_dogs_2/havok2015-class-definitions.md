# Havok 2015.1 Class Definitions (WD2)

Extracted from `Disrupt_64.dll` — 235 classes with sizes, versions, and parent chains.
Member arrays are in the encrypted `.link` section (VMProtect); full layouts require
a runtime dump.

Sample structs (from `Havok2015_WD2.h`):

```c
struct hkAlignSceneToNodeOptions : public hkReferencedObject {
    // size: 40 bytes, version: 7
};

struct hkGeometry : public hkReferencedObject {
    // size: 48 bytes, version: 7
    // 6 members (encrypted .link)
};

struct hkMeshBody : public hkReferencedObject {
    // size: 440 bytes, version: 7
    // 3 members (encrypted .link)
};

struct hkMeshSystem : public hkReferencedObject {
    // size: 240 bytes, version: 7
    // 1138 members (encrypted .link)
};

struct hknpBodyCinfo {
    // size: 256 bytes, version: 7
    // 22 members (encrypted .link)
};
```

> **Source:** Havok 2015.1 class definitions (235 classes) extracted from WD2's Disrupt_64.dll.
> Full header: `WD2/Havok2015_WD2.h`
> **Cross-reference:** WD2 HKX/TAG0 format → [`wd2-tag0-collision-format.md`](wd2-tag0-collision-format.md); WDL Dunia wrapper + Havok 2017.2 → [`../watch_dogs_legion/hkx_format_wdl.md`](../watch_dogs_legion/hkx_format_wdl.md)
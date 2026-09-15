# XBGMIP Format — High-Detail Streamed LOD (WD1)

> **Source**: blender-io-disrupt addon parser (`modules/Watch_Dogs/import_wd.py:391-430`),
> injector (`modules/Watch_Dogs/inject_wd.py:166-376`), verified byte-exact on
> `helicopter_01.high.xbgmip`.

## Purpose

WD1 vehicles and some clothing meshes use **streamed LOD** — the `.xbg` file contains
lower-detail LODs, while the highest-detail LOD0 lives in a companion `.high.xbgmip`
file. The game streams from the xbgmip as the player approaches. Path is embedded in
the XBG's `GeomMips` section; the file sits next to the `.xbg` with `.high.xbgmip`
suffix.

## File Layout

```
Offset  Size   Field
------  ----   -----
 0      4      magic            'PIMG' (0x50494D47)
 4      4      version          u32 = 1
 8      4      reserved         u32 = 0
12      4      n_buffers        u32 = 1 (always single-buffer)
16      4      vsize            u32 = vertex data size in bytes
20      vsize  vertex_data      packed vertex block
20+v    4      isize            u32 = index data size in bytes
24+v    isize  index_data       packed u16 index block
```

**Total file size** = `20 + vsize + 4 + isize`

All fields are little-endian.

## Fields

| Field | Type | Description |
|-------|------|-------------|
| `magic` | char[4] | Always `PIMG`. Used to identify the format. |
| `version` | u32 | Format version. Known: `1`. |
| `reserved` | u32 | Always `0`. Purpose unknown. |
| `n_buffers` | u32 | Number of vertex/index buffer pairs. Always `1` in observed files. |
| `vsize` | u32 | Byte length of `vertex_data`. |
| `vertex_data` | u8[vsize] | Packed vertex data. Layout matches the XBG's LOD0 vertex format (stride, format, drawcall offsets from the parent XBG). |
| `isize` | u32 | Byte length of `index_data`. |
| `index_data` | u16[isize/2] | Triangle strip/fan indices. Same format as XBG index buffers. |

## Vertex Data

The `vertex_data` block uses the **same vertex format** as the parent XBG's LOD0
drawcalls — same stride, same attribute layout (position, normal, UV, tangent, etc.).
The injector patches this block byte-for-byte when modifying vertex positions.

Key detail from the injector: vertex data offset within the buffer is `dc['vb_offset']`
(drawcall vertex buffer offset from the XBG mesh table). The xbgmip buffer is treated
as buffer[0] in the LOD system.

## Index Data

u16 index buffer. Same format as XBG index buffers — indices into the vertex data.
The injector re-encodes these when vertex counts change.

## Relationship to XBG

```
vehicle.xbg          ← contains LOD1, LOD2, ... (lower detail)
vehicle.high.xbgmip  ← contains LOD0 (highest detail, streamed)
```

The XBG's `GeomMips` section lists LODs. When `skip >= 1` (meaning the XBG is
missing LOD0), the parser looks for `<base>.high.xbgmip` and prepends it to the
buffer list, restoring LOD0 as buffer[0].

## Injector Behavior

The blender-io-disrupt injector (`inject_wd.py`) patches xbgmip files alongside the
XBG:

1. Reads the companion `.high.xbgmip` as a `bytearray`
2. Patches vertex data in-place (same offset/stride as the XBG's LOD0 drawcalls)
3. If vertex count changes, calls `_maybe_expand_bounds()` to resize the buffer
   sections (shifts index data, updates `vsize`/`isize` header fields)
4. Writes the patched buffer to `<out_base>.high.xbgmip` next to the output XBG

**Important**: Without patching the xbgmip, the game streams the pristine hi-res
copy over your edit as you approach — the "my edit reverted" bug.

## Workaround (No Reverse Engineering Needed)

FrankMK04's workaround avoids understanding the format entirely:

1. Delete the xbgmip hash from the XBG file (hex edit)
2. Replace `high.xbgmip` with your modified XBG (renamed to `.high.xbgmip`)
3. Game cannot find the original high-res, so it uses your modified XBG as LOD0
4. Original collision detection is retained

## Examples

Observed file: `helicopter_01.high.xbgmip` (verified byte-exact parse)

```
Offset 0:  50 49 4D 47   PIMG
Offset 4: 01 00 00 00    version = 1
Offset 8: 00 00 00 00    reserved = 0
Offset 12: 01 00 00 00   n_buffers = 1
Offset 16: [vsize]       vertex data size
Offset 20: [vsize bytes] vertex data
Offset 20+v: [isize]     index data size
Offset 24+v: [isize bytes] index data
```

## See Also

- [XBG Format (GeomParser)](watch_dogs/xbg-format-geparser.md) — parent format
- [Custom Model Workflow](custom-model-workflow.md) — XBGMIP hack workaround
- [blender-io-disrupt Addon](blender-addon.md) — parser/injector source

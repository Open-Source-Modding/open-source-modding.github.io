HKX (Havok Packfile) format — reference for FO4
================================================

Status: research notes distilled from the Havok 2014 SDK (`hk_2014.1.0-r1`),
Lukas Cone's Havok format library (used only as a factual reference, GPL code
is NOT incorporated), and the FO4 Havok animation guide. Goal: a clean-room
Python reader/writer for Fallout 4 animation `.hkx` files.

This document records format FACTS: field names, sizes, order, and algorithms.
It is written from the formats themselves, not from any single implementation.

Scope
-----
Fallout 4 uses Havok 2014.1.0, contents version string ``hk_2014.1.0-r1``,
packfile version 11, 64-bit pointers, little-endian. This is the "old" (pre
2015) packfile layout. Newer games (2015+) use a different container layout
("new" format, FourCC chunked: DATA/SDKV/TYPE/INDX) which is out of scope here.

> **Confirmed users of the chunked format (2026-08-18):** **Starfield** (Creation
> Engine 2) ships Havok **2019.02** — ``sdk_version`` = ``"20190200"`` — and uses
> the new chunked packfile container (``ChunkType`` records, ``PTCH`` patch
> sections, indexed blocks), **not** the classic FO4-style packfile documented in
> this file. This confirms the "newer games" note above. The chunked layout is
> documented separately (TODO: chunk table, PTCH semantics, indexed-block
> addressing); see ``reference/bethesda/starfield-guide.md`` for the finding and
> implications for FO4-era tooling (hkxcmd/hkxpack cannot read it as-is).

Container layout
----------------
A packfile is a header followed by one or more sections. FO4 files typically
have three: ``__classnames__``, ``__types__`` and ``__data__``. Older files may
carry extra sections (``__classindex__``, ``__dataindex__``) and a version-2
header; the class descriptor index is given by
``contents_classname_section_index`` and the object data section by
``contents_section_index``, so parse by index, not by tag.

Header (the field order is fixed)::

    uint32  magic1              = 0x57e0e057
    uint32  magic2              = 0x10c0c010
    uint32  user_tag
    uint32  version             = 11 for FO4, 8 for Skyrim SE
    uint8   bytes_in_pointer    = 8 (4 for x86 packfiles)
    uint8   little_endian       = 1
    uint8   reuse_padding_optimization
    uint8   base_class          = 1 (always)
    int32   num_sections        = 3
    int32   contents_section_index          = 2      (__data__)
    int32   contents_section_offset         = 0
    int32   contents_classname_section_index = 0    (__classnames__)
    int32   contents_classname_section_offset = 75
    char[16] contents_version   = "hk_2014.1.0-r1\0" for FO4
    uint32  flags
    int16   max_predicate       = -1 (no predicates in FO4)
    int16   section_offset      = -1 (no extra header bytes in FO4)

If ``section_offset == 16``, additional fields follow before the section table:
``int16 unk_40, int16 unk_42, uint32 unk_44, uint32 unk_48, uint32 unk_4c``.
These are game-specific (e.g. BotW WiiU uses them).

If ``little_endian`` is 0 all multi-byte values are byte-swapped.

Section table (one entry per section, 3 sections total in FO4)::

    char[16] section_tag        e.g. "__classnames__", "__types__", "__data__"
    uint32   constant           = 0xFF000000  (00 00 00 FF little-endian)
    uint32   absolute_data_start
    uint32   local_fixups_offset
    uint32   global_fixups_offset
    uint32   virtual_fixups_offset
    uint32   exports_offset
    uint32   imports_offset
    uint32   end_offset
    ... if version >= 11: 16 bytes 0xFF padding AFTER the fields ...

The 16-byte ``0xFF`` padding is present only for packfile version >= 11
(verified against hkxpack ``SectionInterface`` and the FO4 __data__ section:
8 x uint32 fields = 32 bytes + 16 pad = 64 bytes per section entry). Skyrim SE
(version 8) has no padding.

Section contents
----------------
ALL offsets in the section header (``local_fixups_offset``,
``global_fixups_offset``, ``virtual_fixups_offset``, ``exports_offset``,
``imports_offset``, ``end_offset``) are RELATIVE to ``absolute_data_start``
(verified in the Havok 2014 SDK, ``hkPackfileSectionHeader.h``). File offsets
are computed as ``absolute_data_start + relative_offset``.

Section buffer: ``end_offset`` bytes, file range
``[absolute_data_start, absolute_data_start + end_offset)``. The data region
ends where the local fixups begin: data bytes =
``[absolute_data_start, absolute_data_start + local_fixups_offset)``.

Fixup arrays are laid out after the data region, still relative to
``absolute_data_start``. Element counts are derived from the offsets:

* local fixups:   between ``local_fixups_offset`` and ``global_fixups_offset``,
  each ``{ int32 pointer; int32 destination }`` (8 bytes).
* global fixups:  between ``global_fixups_offset`` and ``virtual_fixups_offset``,
  each ``{ int32 pointer; int32 section_id; int32 destination }`` (12 bytes).
* virtual fixups: between ``virtual_fixups_offset`` and the section's "virtual
  EOF", where virtual EOF is ``exports_offset`` if it is not -1 else
  ``imports_offset``, each ``{ int32 data_offset; int32 section_id;
  int32 classname_offset }`` (12 bytes).

Fixup semantics (all pointer/destination fields are 32-bit offsets relative
to the owning section's ``absolute_data_start``, even for 64-bit builds):

* local fixup: write the address of (section buffer + destination) at
  (section buffer + pointer). Used for pointers into the same section.
* global fixup: write the address of
  (sections[section_id] buffer + destination) at (section buffer + pointer).
  Used for pointers into other sections (e.g. class data -> classnames).
* virtual fixup: creates a virtual class object. Read the class name from
  ``sections[section_id]`` buffer at ``classname_offset``, compute its Jenkins
  one-at-a-time hash, look up the class descriptor, then parse the class data
  starting at ``data_offset``.

Entries whose first field (``pointer``/``data_offset``) is ``-1``
(``0xFFFFFFFF``) are padding and MUST be skipped (verified in the SDK reader,
``hkBinaryPackfileReader.cpp``). A virtual fixup ``classname_offset`` points at
the class name string; in the ``__classnames__`` section each name is preceded
by a 4-byte UUID and a ``0x09`` breaker byte, so
``classname = sections[sec].data[classname_offset:]`` reads the name directly
(no need to skip the UUID — the fixup already points past it).

Verified example (FO4 ``Block.hkx``, ``__data__`` section, abs start = 496,
version 11): local fixups at file 10096 (110 x 8-byte pairs), global fixups at
file 10976 (5 x 12-byte entries + 4 bytes 0xFFFFFFFF padding), virtual fixups
at file 11040 (5 x 12-byte entries + 4 bytes 0xFFFFFFFF padding),
``end_offset`` lands exactly at EOF (496 + 10608 = 11104).

Virtual fixups found: ``(0,0,75) (96,0,101) (224,0,128) (9360,0,162)
(9488,0,200)`` — all in section 0 (``__classnames__``); offset 101 yields
"hkaAnimationContainer", 128 "hkaSplineCompressedAnimation", 162
"hkaDefaultAnimatedReferenceFrame", 200 "hkaAnimationBinding", 75
"hkRootLevelContainer".

Object model / pointers
-----------------------
A virtual class has this layout in memory (what fixups point at)::

    ptr      vtable
    int16    mem_size_and_flags
    int16    reference_count
    ... member data ...

Not every object is a virtual class. Plain structs are stored inline. Pointers
between objects are stored as fixups, so reading an object requires following
the pointer slot to the fixup that targets it.

Data alignment: 8 bytes for pointer-sized members on 64-bit, 4 otherwise.
Havok's "reuse padding optimization" lets derived classes reuse the padding of
non-POD bases (this is why struct sizes are not obvious from C++ alone).

Class descriptors
-----------------
Class names map to a hash (Jenkins one-at-a-time, see below) that indexes a
registry. The concrete class data for the common FO4 animation objects:

String pointer
    A pointer to a NUL-terminated ASCII string (fixup target). Every object
    that references strings does so through pointer fixups.

``hkArray<T>`` (HK700+, i.e. everything relevant to FO4)::

    ptr       data              (fixup target)
    uint32    count
    uint32    capacity_and_flags

Pre-700 files use ``LegacyArray``: a pointer plus a ``uint32`` count with NO
capacity field, and a different member order (pointer before count). FO4
animations/skeletons are HK700+, use the 3-field form.

``hkReferenceObject`` (base class of most runtime objects)::

    ptr      vtable
    int16    mem_size_and_flags
    int16    reference_count

``hkQTransform`` — 48 bytes, 16-byte aligned::

    hkQuaternion rotation   (4x float32, 16 bytes)
    hkVector4   translation (4x float32)
    hkVector4   scale       (4x float32)

``hkVariant``::

    ptr   object
    ptr   class_description

``hkNamedVariant``::

    ptr   name         (string)
    ptr   class_name   (string)
    ptr   variant      (-> hkVariant object)   [HK700+]

``hkRootLevelContainer``::

    hkArray<hkNamedVariant> variants

``hkaBone``::

    ptr    name            (string)
    int32  lock_translation

``hkaSkeleton`` (HK700+ form, what FO4 uses)::

    hkReferenceObject  inherited
    ptr                name          (string)
    hkArray<int16>     parent_indices
    hkArray<hkaBone>   bones          (struct array, no fixups between)
    hkArray<hkQTransform> transforms
    hkArray<float>     reference_floats          (HK2010+)
    hkArray<ptr>       float_slots               (HK550+)
    hkArray<hkLocalFrameOnBone> local_frames     (HK600+)
    hkArray<hkaPartition> partitions             (HK2012+)

Order of base->derived members: base class data first, then this list.
``parent_indices`` are int16; -1 means root.

``hkaPartition``::

    ptr     name            (string)
    int16   start_bone_index
    uint16  num_bones

``hkLocalFrameOnBone``::

    ptr     local_frame     (-> hkLocalFrame)
    int32   bone_index      (int16 in HK2011_1 and older)

``hkaAnimationContainer``::

    hkReferenceObject      inherited   (HK700+)
    hkArray<ptr>  skeletons
    hkArray<ptr>  animations
    hkArray<ptr>  bindings
    hkArray<ptr>  attachments
    hkArray<ptr>  skins

``hkaAnnotationTrack``::

    ptr                    name        (string)
    hkArray<hkaAnnotation> annotations

``hkaAnnotation``::

    float  time
    ptr    text           (string)

``hkaAnimation`` (base)::

    hkReferenceObject  inherited
    uint32  animation_type
    float   duration
    uint32  num_of_transform_tracks
    uint32  num_of_float_tracks        (HK550+)
    ptr     extracted_motion           (-> hkaAnimatedReferenceFrame)
    hkArray<hkaAnnotationTrack> annotations   (HK700+; NOTE: member array,
                                              each element carries its own name ptr)

Animation type enum (2011+, applies to FO4)::

    0 = HK_UNKNOWN_ANIMATION
    1 = HK_INTERLEAVED_ANIMATION
    2 = HK_MIRRORED_ANIMATION
    3 = HK_SPLINE_COMPRESSED_ANIMATION
    4 = HK_QUANTIZED_COMPRESSED_ANIMATION
    5 = HK_PREDICTIVE_COMPRESSED_ANIMATION
    6 = HK_REFERENCE_POSE_ANIMATION

``hkaSplineCompressedAnimation`` (size: 176 bytes, signature: 0x792ee0bb)::

    hkaAnimation   inherited
    uint32  num_frames                    offset: 56
    uint32  num_blocks                    offset: 60
    uint32  max_frames_per_block          offset: 64
    uint32  mask_and_quantization_size    offset: 68
    float   block_duration                offset: 72
    float   block_inverse_duration        offset: 76
    float   frame_duration                offset: 80
    uint32  (padding)                     offset: 84
    hkArray<uint32> block_offsets         offset: 88
    hkArray<uint32> float_block_offsets   offset: 104
    hkArray<uint32> transform_offsets     offset: 120
    hkArray<uint32> float_offsets         offset: 136
    hkArray<uint8>  data                  offset: 152
    int32   endian                        offset: 168
    uint32  (padding)                     offset: 172

``hkaAnimationBinding``::

    hkReferenceObject    inherited    (HK700+)
    ptr                  original_skeleton_name (string)  (HK600+)
    ptr                  animation    (-> hkaAnimation)
    hkArray<int16>       transform_track_to_bone_indices
    hkArray<int16>       float_track_to_float_slot_indices  (HK550+)
    hkArray<int16>       partition_indices                  (HK2012+)
    uint8                blend_hint

The XML/reflection name for the skeleton string is ``originalSkeletonName``
(not "skeletonName"). The blend hint enum (Havok ``BlendHint``): 0 = normal,
1 = additive, 2 = from previous, 3 = combine. A track index of 0xFFFF (65535)
means the track maps to nothing.

``hkaAnimatedReferenceFrame`` variants exist (default / delta) but FO4 anims
usually reference extracted motion through this pointer type.

Jenkins one-at-a-time hash
--------------------------
Used to map class names to descriptors::

    hash = 0
    for c in name_bytes:
        hash += c
        hash += hash << 10
        hash ^= hash >> 6
    hash += hash << 3
    hash ^= hash >> 11
    hash += hash << 15

Returned as an unsigned 32-bit value.

Spline-compressed animation data
--------------------------------
The animation's ``data_buffer`` holds concatenated blocks. Each block covers up
to ``max_frames_per_block`` frames across all transform tracks. ``block_offsets``
give byte offsets into ``data_buffer`` for each block.

Per-block layout::

    TransformMask masks[num_tracks]        (see below)
    ... float track data (num_float_tracks bytes) ...
    padding to 4
    per track, in order:
        position track, rotation track, scale track

``TransformMask`` (4 bytes) packs quantization + track-type bits::

    uint8 quantization_types
    uint8 position_types      (bit flags, see below)
    uint8 rotation_types      (low nibble static flag, high nibble spline flag)
    uint8 scale_types

Quantization bits in ``quantization_types``::

    bits 0-1   position quantization:  0=8bit, 1=16bit
    bits 2-5   rotation quantization:  value+2 -> 2=32bit, 3=40bit, 4=48bit,
                                      5=24bit, 6=16bit-quat, 7=uncompressed
    bits 6-7   scale quantization:     0=8bit, 1=16bit

Position/scale type flags (bit 0 = X static, bit 1 = Y static, bit 2 = Z
static, bit 4 = X spline, bit 5 = Y spline, bit 6 = Z spline). A channel is:
* identity if no static/spline bit set,
* static if the static bit is set (one stored value, no spline),
* dynamic (spline) if the spline bit is set.

Rotation: low nibble of ``rotation_types`` set = static; high nibble set =
spline (dynamic). Otherwise identity.

Track encodings (dynamic):
    Write order: a ``uint16`` item count, one skip byte, then a ``uint8``
    spline degree, then ``num_items + degree + 2`` knot bytes, then padding.
    Then per channel that is dynamic: a ``{float min, float max}`` bounding
    box (8 bytes), then ``num_items + 1`` samples. Samples are stored as
    normalized bytes (position/scale: 8-bit or 16-bit) mapped into the
    channel's bounding box: ``value = min + (max - min) * (sample / 255)``
    (or / 65535 for 16-bit).

Static channels store a single float directly (4 bytes). Identity channels
store nothing.

Rotation track (dynamic): item count + degree + knots as above (knot padding
aligns to 2 for 48-bit / 16-bit-quat, to 4 for 32-bit / uncompressed), then
``num_items + 1`` quantized quaternions.

Quaternion quantization (all store the rotation directly, normalized via
sign/magnitude encoding):
* 32-bit: 10-bit magnitude ``R`` in the top bits, 18-bit ``phi/theta`` split
  in the low bits; sign bits select which of x/y/z/w is negative.
* 40-bit: three 11-bit components + sign/w bits; component order permuted by
  a 2-bit ``result_shift``.
* 48-bit: three 15-bit components (signed) + sign/w bits; same permutation.
* 16-bit quat: 8-bit components.
* uncompressed: 4 float32 (16 bytes).

Spline evaluation (NURBS)
-------------------------
Standard NURBS basis (this is the classic "NURBS Book" De Boor formulation):
* control points: ``num_items + 1`` points, degree ``d``, knot vector of size
  ``num_items + d + 2``.
* knot span lookup + De Boor (or Cox-de Boor) evaluation produce the value at
  time ``t`` normalized to the local block frame.
* ``block_duration`` / ``block_inverse_duration`` / ``frame_duration`` convert
  global animation time into a local block frame (``local = (t - block_start)
  * block_inverse_duration * max_frames_per_block``).

Decoding a frame of a spline animation
--------------------------------------
1. Determine the block index from the frame number (frame / max_frames_per_block).
2. Locate the block via ``block_offsets[block]``.
3. Parse the block's masks, tracks, and control points (above).
4. For each transform track, evaluate position, rotation, scale at the local
   frame and combine into an ``hkQTransform``.
5. Map transform tracks to bones via ``hkaAnimationBinding.transform_track_to_bone_indices``.

Delta / interleaved animations
------------------------------
Interleaved and delta-compressed animations are out of scope for a first
implementation (FO4 uses spline compression for the vast majority of
animations). The interleaved format stores per-track data uncompressed; delta
stores per-block delta frames. The SDK's ``hka_interleaved*`` and delta
headers are the reference for a later pass.

Validation strategy
-------------------
Ground truth for testing: unpack real FO4 .hkx with hkxpack (MIT-licensed,
external tool) to XML, compare against our parser's output for the same file.
The XML uses class names, not hashes, which makes diffing straightforward.

Licensing note
--------------
This spec is derived from public file-format facts (field names, sizes,
algorithms) cross-checked against the Havok 2014 SDK and the FO4 modding
community documentation. It does not copy GPL implementation code. Any Python
written against this spec is original work.
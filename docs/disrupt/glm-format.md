# GLM Format (Watch Dogs / Disrupt Engine)

## Overview

GLM is the **text-based source format** for 3D models in the Disrupt engine (Watch Dogs 1/2/Legion). It is the input that the Disrupt editor compiles into XBG (compiled binary mesh). GLM is NOT binary — it's a tab-separated, CRLF-terminated text format.

**72,758 GLM files** found in the leak (`leak/ubisoft/data/`).

## Relationship to XBG

```
GLM (text source) → Disrupt Editor → XBG (compiled binary) → Game
```

GLM files exist in the leak's source data directories. The compiled XBG files are in `data_win64/` archives. There is no 1:1 correspondence in the leak (GLM sources are separate from cooked XBG outputs).

## Format Structure

Tab-separated key-value pairs, CRLF line endings. Uses `{`/`}` for nested blocks.

### Top-Level Keys

```
VERSION\t11.00000000
TYPE\t"GEOM"
OBJECT_NAME\t"DoorDouble_Commercial_01"
DYNAMIC_POSITION_COMPRESSION_RANGE\t1
POSITION\t0.00000000\t0.00000000\t0.00000000
ROTATION\t0.00000000\t0.00000000\t0.00000000\t0.00000000
```

- **VERSION**: Format version (9.0000000 to 11.0000000 observed)
- **TYPE**: Always `"GEOM"` for geometry objects
- **POSITION**: 3D vector (x, y, z) — object origin
- **ROTATION**: Quaternion (x, y, z, w)

### MATERIAL_REFERENCE_LIST

```
MATERIAL_REFERENCE_LIST\t{
\tNB_MATERIAL\t1
\tMATERIAL\t{
\t\tSHADER\t"WD2Generic"
\t\tSHADERREFID\t"graphics_materials\pfernandez-M-2013021536191681.material.xml"
\t\tSLOTNAME\t"Door_Commercial_01"
\t}
}
```

- **SHADER**: Shader name (e.g., "WD2Generic", "character_skin", "character_hair")
- **SHADERREFID**: Path to material XML definition
- **SLOTNAME**: Material slot name for mesh assignment

### SKELETON_LIST

```
SKELETON_LIST\t{
\tNB_SKELETONS\t1
\tSKELETON\t{
\t\tNB_BONE\t423
\t\tBONE\t{
\t\t\tNAME\t"Pelvis"
\t\t\tPARENT\t"Root"
\t\t\tPOSITION\t0.00001983\t0.00000000\t0.99202001
\t\t\tROTATION\t0.00688343\t-0.99995166\t-0.00702117\t1.57088232
\t\t\tSCALE\t1.00000000\t1.00000000\t1.00000000
\t\t}
\t}
}
```

- **NB_BONE**: Bone count (0 for rigid props like doors)
- **BONE**: Name, parent bone name, local position (3D), local rotation (quaternion), scale (3D)
- **Root bone**: Parent is empty string or omitted

### REFLEX_SYSTEM (XML embedded)

```
REFLEX_SYSTEM\t{
\t@@@@ BEGIN_EMBEDDED_BLOB "rfxblob_RFX_Legs"
<Reflex Version="2.07" Contents="System">
  <System Name="RFX_Legs">
    <Constraint Type="SingleBalljoint" Name="..." ParentOfSocketBone="..." SocketBone="..." TargetBone="...">
      ...
    </Constraint>
  </System>
</Reflex>
\t@@@@ END_EMBEDDED_BLOB
}
```

- Physics constraint system (ragdoll, secondary motion)
- Embedded XML blob with ball joints, pose hulls, constraints
- Only present in character/skinned models (not rigid props)

### PROCEDURAL_NODES_LIST

Procedural animation nodes (IK, look-at, etc.). Character-specific.

### SECONDARY_MOTION_OBJECT

Secondary motion/physics data (cloth, hair, accessories). Character-specific.

### GEOMETRY_LIST

```
GEOMETRY_LIST\t{
\tNB_GEOMETRY\t2
\tLOD_DISTANCES\t{
\t\tLOD\t0\t40.00000000
\t\tLOD\t1\t250.00000000
\t\tLOD\t2\t0.00000000
\t\tLOD\t3\t0.00000000
\t\tLOD\t4\t0.00000000
\t\tLOD\t5\t0.00000000
\t}
\tQUALITY_SPLIT\t{
\t\tGENERATEHIGHRES_NG\t1
\t\tGENERATEMEDIUMRES_NG\t1
\t\tGENERATEHIGHRES_CG\t1
\t\tGENERATEMEDIUMRES_CG\t1
\t}
\tTRIMESH\t{
\t\tMESH_NAME\t"DoorDouble_Commercial_01_LOD0"
\t\tMESH_STATE_INDEX\t-1
\t\tNUM_TVERTEX_CHANNEL\t1
\t\tNUM_CVERTEX_CHANNEL\t1
\t\tVERTEX_LIST\t{
\t\t\tNB_VERTEX\t150
\t\t\tVERTEX\t0\t1.30170655\t0.03000019\t0.00000000
\t\t}
\t\t...
\t}
}
```

#### TRIMESH Components

- **MESH_NAME**: Mesh identifier (e.g., "DoorDouble_Commercial_01_LOD0")
- **NUM_TVERTEX_CHANNEL**: Number of texture coordinate channels (UV sets)
- **NUM_CVERTEX_CHANNEL**: Number of vertex color channels
- **VERTEX_LIST**: Position data (index, x, y, z)
- **TV_LIST**: Texture coordinates (index, u, v) per channel
- **VC_LIST**: Vertex colors (index, r, g, b, a) per channel
- **FACE_LIST**: Triangle indices (vertex, UV, vertex color per corner)
- **NORMAL_LIST**: Normal vectors (index, nx, ny, nz)
- **TANGENT_LIST**: Tangent vectors
- **BINORMAL_LIST**: Binormal vectors

#### Face Format

```
FACE\t0\t0\t1\t3\t0\t1\t2\t0\t3\t1\t-1\t-1\t-1\t-1\t-1\t-1\t-1\t-1\t-1\t1\t0
CFACE\t0\t0\t-1\t-1\t-1
```

- **FACE**: face_index, v0, v1, v2, uv0_0, uv0_1, uv0_2, uv1_0, uv1_1, uv1_2, ... (vertex indices, UV indices per channel, material ID)
- **CFACE**: face_index, color_channel_index, c0, c1, c2 (vertex color indices)

## Character Models

Character GLMs (e.g., `char01.glm`) are significantly larger (~1.18M lines) and contain:
- Full skeleton (423 bones for humanoids)
- REFLEX_SYSTEM (ragdoll constraints)
- PROCEDURAL_NODES_LIST (IK, procedural animation)
- SECONDARY_MOTION_OBJECT (cloth, hair physics)
- Multiple TRIMESH blocks (body parts, clothing)

## Usage for Modding

### Import Path (GLM → Blender)
1. Parse GLM text format (Python)
2. Extract vertices, faces, UVs, normals, bones, materials
3. Create Blender mesh + armature + materials

### Export Path (Blender → GLM → XBG)
1. Export from Blender to GLM text format
2. Use Disrupt editor to compile GLM → XBG
3. OR use our XBG exporter directly (bypasses GLM)

### Alternative: Direct XBG Export
Our blender-io-disrupt addon exports XBG directly from Blender, bypassing GLM entirely. GLM import is useful for:
- Importing beta/source models not yet compiled to XBG
- Editing existing GLM sources in the leak
- Understanding model structure (GLM is human-readable)

## Files

- Leak source: `leak/ubisoft/data/graphics/` (72,758 GLM files)
- Character models: `leak/ubisoft/data/graphics/Characters/char/char01/char01.glm`
- Prop models: `leak/ubisoft/data/graphics/buildings/door/DoorDouble_Commercial_01.glm`

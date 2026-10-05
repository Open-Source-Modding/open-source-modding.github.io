# Character Model & Outfit Modding (WD1)

Source: guru3D "Watch Dogs - customOutfits mod" thread (The Silver, 2015-10-06).

How outfits and character models are assembled, and where each layer of the chain can be edited. Everything here is `.lib` editing: the model files themselves are not rebuilt, you repoint existing pieces or bolt extra parts onto an existing model.

## Outfit items in `items.lib`

The outfits in the shop are described here, for example `Clothing_SP.DefaultSkin.SP_Cloth_Store_Aiden_01.xml`. You can create more of these files. The only restriction is that the key in this line must be unique:

```xml
<field name="hidKey" type="StringId">0x876BED86</field>
```

A new file may or may not result in another item in the shop. It works for cars in the car-on-demand app, but not 100% reliably for outfits.

Useful fields:

**GraphicKit model ID**

```xml
<field name="graphickitmodelModel" type="BinHex">84504AEC</field>
```

The ID of the model loaded when you choose the outfit. This is the first point where outfits can be changed: `graphickit_models.lib` contains thousands of NPC models plus the main characters and Aiden's outfits, so if you know a model ID you can put it here and that model loads when you change clothes.

**Name**

```xml
<field name="LocalizationId" type="UInt32">195863</field>
```

Game text cannot be edited yet, but replacing this ID with another item's, car's or bike's ID gives that item's name.

**Icon**

```xml
<field name="sItemIconTextureName" type="String">Aiden/Aiden_kit_01</field>
```

XBT images live in `\ui\fire\sources\textures\nexus_items\shop\aiden\`.

**Availability**

```xml
<field name="accessidAccessIdToGiveItem" type="BinHex">FF</field>
```

Setting this to `FF` gives access to unlockable or DLC items.

**Shop price**

```xml
<field name="uiItemPrice" type="UInt32">1000</field>
```

## Model definitions in `graphickit_models.lib`

These files define the models: every NPC, every Aiden variant, every main character. Some are single models, some are composed of several parts.

Main elements:

- header: name, key, and other unknown fields
- model parts, starting with a line like `<object hash="A88188FA">`. Single models have one, composed models have several:

  ```xml
  <object hash="E93DDFF8">
    <field hash="6816806D" type="BinHex">03</field>
    <field hash="D935FAD9" type="BinHex">57D3BCBC</field> <!-- graphickit part ID -->
    <field hash="14088DCA" type="BinHex">FF</field> <!-- unknown, usually FF but sometimes another value -->
    <field hash="73CEC34A" type="BinHex">1E7C8A1A</field> <!-- material variation of the model part -->
    <object hash="37CF95BD">
      <field hash="CF1DBEC6" type="BinHex">FFFFFFFF</field>
      <field hash="5177A0C7" type="BinHex">000000000000000000000000</field> <!-- position (z,x,y) -->
      <field hash="99298BDA" type="BinHex">000000000000000000000000</field> <!-- rotation (z,x,y) -->
      <field hash="FC176C73" type="BinHex">0000803F0000803F0000803F</field> <!-- scale (z,x,y) -->
    </object>
  </object>
  ```

- near the end of the file the parts appear again, one line each
- at the very end come the three main parts again: head, torso, legs. For single-object models the head and legs rows are empty.

If you know a model part ID and a material variation ID you can add that part to any model, and any number of parts can be added. Position, rotation and scale cannot be changed on skinned meshes, so anything animated (faces, jackets, etc.) ignores these values. Hats and accessories do work.

## Model parts in `graphicktit_parts.lib`

This is where every model part is described.

```xml
<field name="hidKey" type="StringId">0xBCBCD357</field> <!-- model part ID referenced from the graphickit_model files -->
```

Note the reversed byte order. The file may contain a list of material overrides, which are the material variations of the part. Each override entry has a header carrying the ID referenced from the graphickit_model files:

```xml
<field name="Name" type="StringId">0x1A8A7C1E</field> <!-- reversed byte order again -->
```

Material overrides usually contain this line, which Disrupt does not translate:

```xml
<field hash="9301DFBD" type="BinHex">67726170686963735C5F6D6174657269616C735C6A6D6172636F75782D6D2D32303133303731313137333733372E6D6174657269616C2E62696E00</field>
```

Convert it with a hex-to-ASCII tool (Notepad++ with the HEX editor plugin, or `Razor.DataConversion`) to read the material it points at:

```xml
<field hash="9301DFBD" type="BinHex">graphics\_materials\jmarcoux-m-20130711173737.material.bin</field>
```

That tells you which `material.bin` the model part uses. Some files live in the `_UNKNOWN` folder, which makes finding the right one painful.

## `materials.bin`

Cannot be unpacked yet, but the contents are readable in any text editor and list the materials. It is possible to rename the file references inside to swap materials.

## Textures (`.xbt`)

The material bin tells you which texture to edit. Several types exist: definition, normal map, shader. Convert with `xbt2dds`, edit the DDS in Photoshop (with the DDS plugin), then convert back with `dds2xbt`.

## Related notes from the same thread

Steps the thread's walkthrough assumes, not present in the standalone text dump:

- Unpack and repack with the Basys/Gibbed tooling before editing any `.lib`.
- A new item's `hidkey` also has to be added to `shopsettings.lib` (`ClothingShop.***.xml`) and any new files added to `items_converted.xml`, or the item will not appear.
- In `graphickit_models.lib` the header field `389F6DA7` is referenced from `items.lib` and has to be unique.
- In `material.bin`, the shader type sits near the start of the file.

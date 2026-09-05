# WD2 Model Hash Mappings

Model hashes map CRC64 identifiers to XBG geometry paths. These are the
`fileModel` values used in entity XML files (`WD2_*.xml`) to reference 3D models.

## Format

Each line maps a model hash to its XBG path:

```
XML_FILE         MODEL_HASH                      CRC64_BYTES     XBG_PATH
W2BK_Vegetation.xml  0x80000007defa4dd1.model  7D E4 FA DE ...  graphics\_geometries\vegetation\static\american_aloe\american_aloe_kit_01_med_01_bk.xbg
```

Fields:
- **XML source file** — entity definition XML that references this model
- **Model hash** — `0x8000000` prefix + CRC64 of the XBG path (WD2 uses CRC64, not FNV1a32)
- **CRC64 bytes** — raw 8-byte hash in hex
- **XBG path** — backslash-separated path to the `.xbg` geometry file

## Hash scheme

WD2 model hashes use CRC64 with mask: `(hash & 0x1FFFFFFFFFFFFFFF) | 0xA000000000000000`.

```python
def model_hash(path: str) -> int:
    text = path.replace("/", "\\").lower()
    num = 14695981039346656037
    for c in text:
        num *= 1099511628211
        num ^= ord(c)
    return (num & 0x1FFFFFFFFFFFFFFF) | 0xA000000000000000
```

## Vegetation models (W2BK_Vegetation.xml)

```
W2BK_Vegetation.xml,  0x80000007defa4dd1.model  graphics\_geometries\vegetation\static\american_aloe\american_aloe_kit_01_med_01_bk.xbg
W2BK_Vegetation.xml,  0x800000024c5c4526.model  graphics\_geometries\vegetation\plants\ficus_plant\ficus_plant_01.xbg
W2BK_Vegetation.xml,  0x80000009b7e6b7d7.model  graphics\_geometries\vegetation\static\cactus_pot_kit_01\cactus_pot_kit_01_large_01.xbg
W2BK_Vegetation.xml,  0x80000002a24c7879.model  graphics\_geometries\vegetation\plants\franciscan_wallflower_kit_01\franciscan_wallflower_kit_01_small_01.xbg
W2BK_Vegetation.xml,  0x80000001e5b49d18.model  graphics\_geometries\vegetation\plants\flowers_generic\flowers_generic_01.xbg
W2BK_Vegetation.xml,  0x80000002a24c785c.model  graphics\_geometries\vegetation\static\platters_tree_kit_01\platters_tree_kit_01_short_01.xbg
W2BK_Vegetation.xml,  0x800000019a65c485.model  graphics\_geometries\vegetation\plants\banana_plant\banana_plant_03.xbg
W2BK_Vegetation.xml,  0x80000001e5b467dd.model  graphics\_geometries\vegetation\plants\banana_plant\banana_plant_03.xbg
W2BK_Vegetation.xml,  0x80000002a24c7867.model  graphics\_geometries\vegetation\static\boxwood\boxwood_straight_120cm_x1.xbg
W2BK_Vegetation.xml,  0x80000001a5714385.model  graphics\_geometries\vegetation\plants\alocasia\alocasia_01.xbg
W2BK_Vegetation.xml,  0x80000002a24c7872.model  graphics\_geometries\vegetation\plants\douglas_iris_kit_01\douglas_iris_kit_01_small_01.xbg
W2BK_Vegetation.xml,  0x80000007defa9cd9.model  graphics\_geometries\vegetation\static\senita_cactus_kit_01\senita_cactus_kit_01_large_01_bk.xbg
W2BK_Vegetation.xml,  0x80000001951b886e.model  graphics\_geometries\vegetation\static\jade_plant\jade_plant_01.xbg
W2BK_Vegetation.xml,  0x80000007defa4dd0.model  graphics\_geometries\vegetation\static\american_aloe\american_aloe_kit_01_large_01_bk.xbg
W2BK_Vegetation.xml,  0x800000019a65c487.model  graphics\_geometries\vegetation\plants\cordyline\cordyline_01.xbg
W2BK_Vegetation.xml,  0x80000007defa4dd4.model  graphics\_geometries\vegetation\static\bunny_cactus_kit_01\bunny_cactus_kit_01_small_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007c5d7d8f8.model  graphics\_geometries\vegetation\plants\fern\fern_kit_01_single_02_bk.xbg
W2BK_Vegetation.xml,  0x80000001e5b50c8c.model  graphics\_geometries\vegetation\plants\phoenix\phoenix_01.xbg
W2BK_Vegetation.xml,  0x80000007defa9d7c.model  graphics\_geometries\vegetation\plants\indian_paintbrush_kit_01\indian_paintbrush_kit_01_small_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007c5d7d8f5.model  graphics\_geometries\vegetation\plants\fern\fern_kit_01_group_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007c5d7d8f6.model  graphics\_geometries\vegetation\plants\fern\fern_kit_01_group_02_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa9b98.model  graphics\_geometries\vegetation\static\white_sage_kit_01\white_sage_kit_01_small_01_bk.xbg
W2BK_Vegetation.xml,  0x800000019a65c482.model  graphics\_geometries\vegetation\plants\areca_palm\areca_palm_01.xbg
W2BK_Vegetation.xml,  0x80000007defa9c0e.model  graphics\_geometries\vegetation\static\american_aloe\american_aloe_kit_01_large_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa9bbd.model  graphics\_geometries\vegetation\plants\california_poppy\california_poppy_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa9c88.model  graphics\_geometries\vegetation\static\bunny_cactus_kit_01\bunny_cactus_kit_01_large_01_bk.xbg
W2BK_Vegetation.xml,  0x80000001e5b467da.model  graphics\_geometries\vegetation\plants\banana_plant\banana_plant_02.xbg
W2BK_Vegetation.xml,  0x800000028afa7de7.model  graphics\_geometries\vegetation\static\cactus_pot_kit_01\cactus_pot_kit_01_large_01.xbg
W2BK_Vegetation.xml,  0x80000007defa9cda.model  graphics\_geometries\vegetation\static\senita_cactus_kit_01\senita_cactus_kit_01_small_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa59db.model  graphics\_geometries\vegetation\static\senita_cactus_kit_01\senita_cactus_kit_01_large_01_bk.xbg
W2BK_Vegetation.xml,  0x80000002a24c7847.model  graphics\_geometries\vegetation\static\white_cedar_pyramidal_kit_01\white_cedar_pyramidal_kit_01_small_01.xbg
W2BK_Vegetation.xml,  0x800000028afa7de6.model  graphics\_geometries\vegetation\static\cactus_pot_kit_01\cactus_pot_kit_01_small_02.xbg
W2BK_Vegetation.xml,  0x800000028afa7de8.model  graphics\_geometries\vegetation\static\cactus_pot_kit_01\cactus_pot_kit_01_large_02.xbg
W2BK_Vegetation.xml,  0x80000007defa9d7b.model  graphics\_geometries\vegetation\plants\cow_parsnip\cow_parsnip_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa59dc.model  graphics\_geometries\vegetation\static\senita_cactus_kit_01\senita_cactus_kit_01_small_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa9c0f.model  graphics\_geometries\vegetation\static\american_aloe\american_aloe_kit_01_med_01_bk.xbg
W2BK_Vegetation.xml,  0x80000003bb2af231.model  graphics\_geometries\vegetation\static\platters_tree_kit_01\platters_tree_kit_01_tall_01.xbg
W2BK_Vegetation.xml,  0x80000007c5d7d8f7.model  graphics\_geometries\vegetation\plants\fern\fern_kit_01_single_01_bk.xbg
W2BK_Vegetation.xml,  0x800000023d43195d.model  graphics\_geometries\vegetation\static\vegetable_garden_kit_01\vegetable_garden_kit_01_lettuce_01.xbg
W2BK_Vegetation.xml,  0x800000023d43195e.model  graphics\_geometries\vegetation\static\vegetable_garden_kit_01\vegetable_garden_kit_01_cabbage_01.xbg
W2BK_Vegetation.xml,  0x8000000246e1f519.model  graphics\_geometries\vegetation\static\vegetable_garden_kit_01\vegetable_garden_kit_01_hot_pepper_01.xbg
W2BK_Vegetation.xml,  0x8000000246e1f518.model  graphics\_geometries\vegetation\static\vegetable_garden_kit_01\vegetable_garden_kit_01_tomato_01.xbg
W2BK_Vegetation.xml,  0x80000001e5b467d9.model  graphics\_geometries\vegetation\plants\banana_plant\banana_plant_01.xbg
W2BK_Vegetation.xml,  0x800000019a65c484.model  graphics\_geometries\vegetation\plants\banana_plant\banana_plant_02.xbg
W2BK_Vegetation.xml,  0x80000009b7e6b7d5.model  graphics\_geometries\vegetation\static\cactus_pot_kit_01\cactus_pot_kit_01_large_02.xbg
W2BK_Vegetation.xml,  0x800000019a65c483.model  graphics\_geometries\vegetation\plants\banana_plant\banana_plant_01.xbg
W2BK_Vegetation.xml,  0x800000028afa7de4.model  graphics\_geometries\vegetation\static\cactus_pot_kit_01\cactus_pot_kit_01_med_01.xbg
W2BK_Vegetation.xml,  0x8000000187bfba90.model  graphics\_geometries\vegetation\static\aloe_vera\aloe_vera_01.xbg
W2BK_Vegetation.xml,  0x800000019a65c489.model  graphics\_geometries\vegetation\plants\red_fountain\red_fountain_01.xbg
W2BK_Vegetation.xml,  0x80000001951b8804.model  graphics\_geometries\vegetation\static\platters_plant\platters_plant_01.xbg
W2BK_Vegetation.xml,  0x80000002a24c7851.model  graphics\_geometries\vegetation\static\platters_tree_kit_01\platters_tree_kit_01_tall_01.xbg
W2BK_Vegetation.xml,  0x800000019a65c486.model  graphics\_geometries\vegetation\plants\flowers_generic\flowers_generic_01.xbg
W2BK_Vegetation.xml,  0x80000007defa4dd3.model  graphics\_geometries\vegetation\static\bunny_cactus_kit_01\bunny_cactus_kit_01_large_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa4dcf.model  graphics\_geometries\vegetation\plants\flax\flax_01_bk.xbg
W2BK_Vegetation.xml,  0x80000001e5b4d5f9.model  graphics\_geometries\vegetation\plants\alocasia\alocasia_01.xbg
W2BK_Vegetation.xml,  0x800000028afa7de5.model  graphics\_geometries\vegetation\static\cactus_pot_kit_01\cactus_pot_kit_01_small_01.xbg
W2BK_Vegetation.xml,  0x80000007defa4dd2.model  graphics\_geometries\vegetation\static\american_aloe\american_aloe_kit_01_small_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa9c62.model  graphics\_geometries\vegetation\static\bunny_cactus_kit_01\bunny_cactus_kit_01_small_01_bk.xbg
W2BK_Vegetation.xml,  0x80000001a56fc016.model  graphics\_geometries\vegetation\plants\cannabis\cannabis_01.xbg
W2BK_Vegetation.xml,  0x80000007defa4ca3.model  graphics\_geometries\vegetation\plants\california_poppy\california_poppy_01_bk.xbg
W2BK_Vegetation.xml,  0x80000007defa9c10.model  graphics\_geometries\vegetation\static\american_aloe\american_aloe_kit_01_small_01_bk.xbg
W2BK_Vegetation.xml,  0x800000019a65c481.model  graphics\_geometries\vegetation\plants\phoenix\phoenix_01.xbg
```

## Character props models (WD2_character_props.xml)

```
WD2_character_props.xml,  0x80000001e2ea0b66.model  graphics\_geometries\character_prop\sign_protest_03.xbg
WD2_character_props.xml,  0x80000005544981a9.model  graphics\_geometries\character_prop\sign_protest_03.xbg
WD2_character_props.xml,  0x80000001b87cba2f.model  graphics\_geometries\character_prop\caulking_gun_01.xbg
WD2_character_props.xml,  0x80000005ccd49694.model  graphics\_geometries\character_prop\sign_protest_04.xbg
WD2_character_props.xml,  0x8000000626ac9c2a.model  graphics\_geometries\character_prop\zippo_01.xbg
WD2_character_props.xml,  0x800000017f6c0be8.model  graphics\_geometries\character_prop\bag_shopping_01.xbg
WD2_character_props.xml,  0x80000003185d83bc.model  graphics\_geometries\character_prop\cloth_cleaning_02.xbg
WD2_character_props.xml,  0x80000001b87f5387.model  graphics\_geometries\character_prop\selfystick_phone_01.xbg
WD2_character_props.xml,  0x80000003185c74ad.model  graphics\_geometries\character_prop\mandoline_01.xbg
WD2_character_props.xml,  0x8000000227d67a7e.model  graphics\_geometries\character_prop\pcparts_kit_01_hdrive_01.xbg
WD2_character_props.xml,  0x80000003185dad8b.model  graphics\_geometries\character_prop\food_burger_box_01.xbg
WD2_character_props.xml,  0x80000001e2ea0b65.model  graphics\_geometries\character_prop\sign_protest_02.xbg
WD2_character_props.xml,  0x80000003185d291e.model  graphics\_geometries\character_prop\crackpipe_01.xbg
WD2_character_props.xml,  0x800000017f6c0b13.model  graphics\objects\_interior\_nikkihouse\nh_adultbedpillow_01.xbg
WD2_character_props.xml,  0x800000019dcd107c.model  graphics\_geometries\locations\_common\interior\headset_01.xbg
WD2_character_props.xml,  0x8000000244e44519.model  graphics\_geometries\interior_furniture\tv_remote_01.xbg
WD2_character_props.xml,  0x8000000244e4b0a3.model  graphics\_geometries\character_prop\wateringcan_01.xbg
WD2_character_props.xml,  0x80000003185d65e1.model  graphics\_geometries\character_prop\food_burger_eaten_01.xbg
WD2_character_props.xml,  0x800000017f6c1861.model  graphics\buildings\_interior\hq\hq_computermouse_01.xbg
WD2_character_props.xml,  0x80000002c788612a.model  graphics\_geometries\character_prop\hand_toolbag_01.xbg
WD2_character_props.xml,  0x8000000205a64921.model  graphics\_geometries\character_prop\guitar_electric_01.xbg
WD2_character_props.xml,  0x8000000227d6b10b.model  graphics\_geometries\character_prop\banjo_01.xbg
WD2_character_props.xml,  0x800000055449ac19.model  graphics\_geometries\character_prop\glass_beer_03.xbg
WD2_character_props.xml,  0x80000002e038bb04.model  graphics\_geometries\character_prop\cin_stencil_dedsec_01.xbg
WD2_character_props.xml,  0x80000001e2ea0b81.model  graphics\_geometries\character_prop\sign_protest_02.xbg
WD2_character_props.xml,  0x80000003185d0239.model  graphics\_geometries\character_prop\flyers_card_01.xbg
WD2_character_props.xml,  0x80000003185d4df7.model  graphics\_geometries\character_prop\badge_pin_01.xbg
WD2_character_props.xml,  0x800000020e9b8267.model  graphics\_geometries\character_prop\saxophone_01.xbg
WD2_character_props.xml,  0x80000001947e3c36.model  graphics\_geometries\character_prop\easel_01.xbg
```

## Full data files

The complete mapping tables (1,501 vegetation + 1,055 character prop entries) are
available as raw text alongside this page:

- `model-hash-mappings-data.txt` — vegetation, fences, street furniture, buildings, etc.
- `model-hash-mappings-character-props.txt` — character prop models

## Related

- `WD2/UnusedCars.txt` — 141 unused/barely-used vehicle hashnames
- `WD2/wd2_depload_info.txt` — depload resource references (material paths, archetype IDs)
- [depload-format.md](depload-format.md) — dependency preload manifest format

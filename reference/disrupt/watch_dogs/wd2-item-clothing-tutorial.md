# WD2 Custom Item/Clothing Creation Tutorial

> **Source**: Discord community tutorials (the task of the Pesky Fly, disastorm, redcomet, CobraGamer, 2016-2023) + XeNTaX forum knowledge.
> **Cross-reference**: XeNTaX WD2 clothing hashes & categories → [XeNTaX Watch Dogs knowledge §8–9, §11](xentax-watchdogs-knowledge.md); Entity XML structure → [`entity-xml-structure.md`](entity-xml-structure.md)

---

## Prerequisites

1. **WD2Extract** + **WD2Pack** (Sir Kane) — extract/repack `patch.fat/dat`
2. **Gibbed.Disrupt.ConvertBinaryObject** — `.lib` ↔ XML conversion
3. Remove WD1 class definitions from `project/Watch Dogs/binary objects/files/gibbed/`
4. Anti-cheat disabled for testing

---

## File Structure Overview

```
patch.fat/dat
├── items.lib              # Clothing/item definitions
├── graphickit_models.lib  # Character model assemblies
├── graphickit_parts.lib   # Individual model parts (heads, torsos, etc.)
├── itemsshopproperties.lib # Shop data (category, price, unlock)
├── shopcomponentsettings.lib # Which items appear in which shops
├── tagcategories.lib      # Category definitions
├── tags.lib               # Tag definitions
└── texturedb.lib          # Texture database references
```

---

## Method 1: Simple Clothing Swap (Replace Existing)

**Source**: disastorm (2016-12-14)

1. Extract `patch.fat/dat`
2. Convert `items.lib` → XML
3. Open clothing XML for source item
4. Open `graphickit_models.lib` → XML for target character
5. Find `D935FAD9` field (graphickit_part reference)
6. Copy the object section containing the reference
7. `grep` on `graphickit_parts` directory to identify what each section represents
8. Replace target sections with source sections
9. Convert back to binary, repack, test

---

## Method 2: Head Swap WITH Animation (redcomet, 2016-12-22)

1. Edit `W2CH_MOD_nar_ava.avatar.xml` in `graphickit_models.lib`
2. Replace head definition with target character's head definition
3. Results in swapped face **with animation**

---

## Method 3: Create New Custom Clothing Item (CobraGamer, 2017-04-08)

### Files to Edit

| File | Purpose |
|------|---------|
| `items.lib` | Create new clothing item with graphickit_part reference |
| `itemsshopproperties.lib` | Define shop category and price |
| `shopcomponentsettings.lib` | Make item appear in specific shop |
| `graphickit_models.lib` | Contains model parts (reference only) |

### Key Hash Fields (from XeNTaX/disastorm)

| Hash | Field | Notes |
|------|-------|-------|
| `389F6DA7` | Item ID | Hex A-F only |
| `D935FAD9` | graphickit_part reference | Links to model part |
| `73280CDE` | Link to item in items.lib | |
| `92AF09E1` | Shop category | |
| `33E4CFD7` | Price | Hex → decimal |
| `24436F9E` | Texture of clothing item | Refers to texturedb.lib |
| `A849532C` | Material of model part | |
| `7E23AAAE` | Model path | Binary hex string |
| `73642E5E` | Graphickit part container | |
| `0F43A933`, `54D14F92`, `4CAAD60F` | Availability rules | Set to `FFFFFFFFFFFFFFFF` to unlock |
| `E7575742` | Category | `FFFFFFFFFFFFFFFF` = uncategorized |

### Clothing Category Values (WD2)

| Hash | Category |
|------|----------|
| `DDD0721E02000080` | NORMCORE |
| `DED0721E02000080` | BIKER |
| `DFD0721E02000080` | URBAN |
| `E0D0721E02000080` | HIPPIE |
| `C61D21C801000080` | HIPSTER |
| `DBD0721E02000080` | SUITS |
| `DCD0721E02000080` | HIP-HOP |

### Step-by-Step: New Clothing Item

1. **Create graphickit_parts entry**
   - Duplicate existing part entry
   - Assign unique IDs
   - Erase all model overrides except one
   - Replace with your custom `.model` (hashed filepath)
   - Point material to your custom `.material.bin` (with new texture paths)

2. **Create items.lib entry**
   - Unique IDs
   - Point to your graphickit_parts entry (`D935FAD9`)
   - Set availability rules to `FFFFFFFFFFFFFFFF`

3. **Extract tagcategories.lib + tags.lib**
   - Find store tag category in tagcategories.lib
   - Find matching store tag in tags.lib
   - Replace `FF...` store tag in graphickit_parts with your store tag

4. **Create itemsshopproperties.lib entry**
   - Unique IDs
   - Point to your items.lib entry (`73280CDE`)
   - Set category hash (`E7575742`) and price (`33E4CFD7`)

5. **Edit shopcomponentsettings.lib**
   - Add your item to desired shop

6. **Create custom model + material**
   - Extract original texture + model
   - Locate original `.material.bin`
   - Duplicate material, point to new texture path
   - Locate original `.model` file
   - Duplicate `.model`, hex edit to point to new material
   - Hash the new filepath for graphickit_parts entry

---

## Method 4: Create New Item Variants (the task of the Pesky Fly, 2023)

### Adding Tutorial

1. Find model in `graphickit_models`
2. Find part by comparing with `graphickit_parts`
3. Create new duplicate entry in `graphickit_parts`, unique IDs
4. Erase all model overrides except one
5. Replace override with your hashed filepath
6. Create new `items.lib` entry, unique IDs, point to custom graphickit_parts
7. Extract `tagcategories.lib` and `tags.lib`
8. Find store tag category in tagcategories.lib
9. Find matching store tag in tags.lib
10. Replace `FF...` store tag in graphickit_parts entry
11. Create new `itemsshopproperties.lib` entry, unique IDs, point to items.lib
12. Edit `shopcomponentsettings.lib` — add item to desired shop

### Creating Variants

1. Extract original texture + model
2. Locate original `.material.bin`
3. Duplicate material bin → point to new texture path
4. Locate original `.model` file
5. Duplicate `.model` file
6. Hex edit `.model` → point to new material
7. Find base model in `graphickit_models`
8. Find base part in `graphickit_parts`
9. Create new duplicate entry in `graphickit_parts`, unique IDs
10. Erase model overrides except one → replace with your `.model`
11. Create new `items.lib` entry → point to custom graphickit_parts
12. Extract `tagcategories.lib` + `tags.lib`
13. Find store tag category → find matching store tag
14. Replace `FF...` store tag
15. Create `itemsshopproperties.lib` entry → point to items.lib
16. Edit `shopcomponentsettings.lib` → add to shop

---

## Important Notes

- **Modified items won't appear in locker** — must equip before adding modded patch
- **Each item maps to ONE graphickit part**
- **After modifying patch, restart game**
- **One item per graphickit part definition**
- **Custom model workflow**: Import → copy rigging → attach to retail model → delete retail parts → keep custom model + retail skeleton → repeat for second pass (single pass = no rigging in game)

---

## Item Category Structure (clothes1.txt)

`itemcategory.lib` defines all potential item categories:

```xml
<object name="libobj">
  <field name="hidSingleObject" type="Boolean">False</field>
  <field name="text_hidName" type="String">Clothing.SP_MOD</field>
  <field name="hidName" type="BinHex">80C15CE9</field>
  <field name="hidKey" type="BinHex">80C15CE9</field>
  <field name="bEnable" type="Boolean">True</field>
  <field name="blgnoreCurrentGen" type="Boolean">False</field>
  <object name="locstridltemCategoryNameld">
    <field name="Localizationld" type="Int64">190602</field>
  </object>
</object>
```

---

## Vehicle Modding (Car on Demand) — neburas, 2016-12-18

- Add vehicles via `CarHackingRewards.Generic.Budjet.XXXX.xml` in `items.lib`
- All Car on Demand vehicles = "player owned" (no wanted level)
- Vehicle archetypes identified by GUID hashes (griffin02, 2017-01-24)

### Traffic Density (Paul44, 2017-03-05)

- Settings in `common.dat` → `trafficpatternselector.lib`
- XML settings allocated to specific map areas (blocks/streets)
- Density values = spawn timing (lower = more frequent)
- File: `generated/databases/generic/trafficpatternselector.lib`
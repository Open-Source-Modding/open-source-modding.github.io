# Adding a New Weapon to Disrupt (WD1)

Source: Discord modding community (DΣΛD_F0X / Silver)

## Basic Weapon Adding

1. **Duplicate a vanilla weapon model** in another folder, save the path to it
2. **weapons.lib** — create a new weapon stats entry: unique name + hidkey, add to `weapons.lib.xml`, repack
3. **items.lib** — create a new weapon item entry: unique name, hidname, hidkey. Point `weaponWeaponParam` to your `weapons.lib` entry. Edit loc string IDs if you have the loc tool. Can adjust price, icon, etc.
4. **items.lib** — generate a new archetype ID (UUID v4), hash it through the bot, compare with vanilla hashed archetype IDs to find the correct hash format
5. **entitylibrary_rt** — add a new weapon archetype by copying an existing entry. Set UID to your hashed archetype ID. Change `disEntityId` to a unique one (start from 9999 — that's the archetype limit)
6. **entitylibrary_rt** — change all `fileModel` hashes to a hash of your weapon model `.xbg` (together with path). Compare with vanilla entries to find the correct hash format
7. **entitylibrary_rt** — change `weaponWeaponPlayer` to the ID of your `weapons.lib` entry
8. **windy_city.depload** — find a vanilla archetype you based your entitylibrary on, duplicate it, set the archetype ID to your generated one, change the `.xbg` it references to your own. In the future you'll have to do this with materials etc. as well

## Living City (LC) Additions

After the basic steps above:

- Adjust your weapon item entry like the LC ones (mostly `maxNbToAdd = 2`)
- Make a corresponding `custom_Weapon...` item with ID = `itemsBaseItem + 1` for Armory
- Add its `hidKey` to the armories `shopsettings` (that's how armories weapons cost $1 and still gives the original weapon item)
- Make a `custom_Weapons...` accessID for it (hidden from armories until you buy the base item in gunshops)
- Make a corresponding `wlu` so you actually drop the weapon upon swapping (optional — otherwise you just won't drop anything)

Follow the pattern of the `Globals_Custom._Weapon_Archetypes` table.

## Additional Libs to Edit

- `iteminventorylists.lib` — add your gun to the proper category
- `shopsettings.lib` — add it to a shop

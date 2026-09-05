# Why Not? — Miscellaneous Ubisoft References

A collection of interesting finds that don't fit neatly into format documentation but provide useful context about Ubisoft's infrastructure, games, and ecosystem.

---

## Ubisoft Game Service (GS) — Complete Game List

Ubisoft used their own Game Service from 2001–2007 for multiplayer/account management. Two implementation methods:

**In-game integration** (`gsconnect.ubisoft.com`): Used from 2002–2007. Games include Far Cry, Splinter Cell: Chaos Theory, Ghost Recon: Jungle Storm, Rainbow Six 3, Brothers in Arms, Silent Hunter III, Heroes of Might and Magic V.

**External ubi.com Game Service Client**: Used 2001–2003, phased out by 2004. Games include Ghost Recon (2001), Rainbow Six: Rogue Spear, IL-2 Sturmovik.

- [Full game list (source)](https://gist.github.com/Joshhhuaaa/94579e63e52eea8c8d2a90f6f8a53b34)

---

## E3 2017 — Press/Streaming Archive Links

Comprehensive index of E3 2017 press portals, Twitch streams, and YouTube archives for all major publishers.

- Ubisoft press: `https://www.ubisoft-press.com/`
- Ubisoft Twitch: `https://www.twitch.tv/ubisoft`
- Ubisoft YouTube: `https://www.youtube.com/c/ubisoft/videos`

- [Full URL list (source)](https://gist.github.com/sebasten/9577dc453f3081d9c477bc6153deca93)

---

## Trackmania on Linux

Trackmania runs on Linux via Proton with no performance overhead. Two installation methods:

- **Steam**: Install via Steam client, enable Steam Play for all titles if needed
- **Lutris**: Use Ubisoft Connect installer, run Openplanet via Wine prefix

Openplanet (extension platform) also works on Linux via Protontricks.

- [Full guide (source)](https://gist.github.com/Geekid812/2f63e6efb4d0dc725487bb0951d09eee)

---

## The Crew — Crash Details (Wine/Linux)

Wine backtrace showing `thecrew` NULL pointer dereference at offset `+0x1630768`. Modules include bink2w64, vulkan-1, uplay_r164, d3d11, dxgi. Running via Wine with Vulkan layers.

- [Source](https://gist.github.com/hcorion/52e0b78dc20e64276b568285a6275c68)

---

## DOF Shader Fix for WD2

ReShade DOF shader (forked from FransBouma) with 16:10 depth buffer correction for cutscenes. Adds `DOF_CORRECT_16_9DEPTH` uniform bool.

- [Source](https://gist.github.com/Selene0623/0f7061a42c8d914b04467be8c22e5dfe)

---

## The Division 2 — Hyprland EAC Self-Kill (bennyfits, 2026)

A Hyprland window-matching script killed The Division 2 before it could start. The EAC splash screen was rebranded to "Tom Clancy's The Division 2", which matched the window criteria for the "close Ubisoft Connect on game close" profile. The splash screen opens → closes → triggers `pkill -f Ubisoft` → game dies.

- [Source](https://gist.github.com/bennyfits/262494a59d163bea750f43ce456e8f67)

---

## UPlay Launch Script — Steam/Proton (IBBoard, 2022)

Shell script to launch Ubisoft Connect via Proton using AC:Liberations' compatdata prefix. Allows playing other Ubisoft games from Steam on Linux. Uses `wine64` from Proton with `WINEDLLOVERRIDES="steam.exe=b;dotnetfx35.exe=b"` and the AC:Liberations Steam prefix.

```bash
LATEST_PROTON=$(ls -1 ~/.local/share/Steam/compatibilitytools.d/Proton*/files/bin/wine64 | grep -v rc | tail -n1)
WINEPREFIX="$HOME/.local/share/Steam/steamapps/compatdata/260210/pfx/" \
    PROTON_USE_D9VK=1 PROTON_NO_ESYNC=1 \
    WINEDLLOVERRIDES="steam.exe=b;dotnetfx35.exe=b" \
    "$LATEST_PROTON" "c:/Program Files (x86)/Ubisoft/Ubisoft Game Launcher/UbisoftConnect.exe"
```

- [Source](https://gist.github.com/IBBoard/99667decc3c04ffc138e8686cbb84730)

## WDL Store Appearance Codes

Complete collection of Watch Dogs: Legion store outfit appearance codes with hex byte patterns, u64 IDs, and texture paths. Covers every outfit in the Outfit Shop and London Dissident Pack.

- Male model ID: 1582, Female model ID: 8624
- Each entry: 28 hex bytes (appearance code) + u64 ID : texture_path
- Useful for outfit modding, texture swaps, and appearance code injection

- [Full code collection (source)](https://gist.github.com/DevLARLEY/9879e7add7b22357a4382a9bc1c7d4da)

---

## Just Dance 2017 on macOS (Whisky)

Running Ubisoft games on Apple Silicon Mac via Whisky (Wine wrapper):

1. Create bottle with Enhanced Sync (ESync) + DXVK enabled
2. Install Steam in the bottle
3. Install game via Steam (triggers Ubisoft Connect install)
4. Uninstall Ubisoft Connect, reinstall version 138.3.10824
5. Replace new Version.txt with saved one from first install

- [Full guide (source)](https://gist.github.com/soopermodel/05db7d16eb5f9d908e3b553f0e1c2fcc)

---

## Trackmania Webservices — Nadeo API Auth

Nadeo's Trackmania API uses Ubisoft's public auth endpoint. Two auth methods:

1. **Ubisoft account**: POST `public-ubiservices.ubi.com/v3/profiles/sessions` with Basic auth (email:password), Ubi-AppId header → get ubi token → POST `prod.trackmania.core.nadeo.online/v2/authentication/token/ubiservices` with `ubi_v1 t=<token>`
2. **Dedicated server account**: POST `prod.trackmania.core.nadeo.online/v2/authentication/token/basic` with Basic auth

Key detail: Ubisoft blocks default user agents. Must include project name + contact in User-Agent header.

- [Full auth guide (source)](https://gist.github.com/codecat/4dfd3719e1f8d9e5ef439d639abe0de4)
- Community docs: https://webservices.openplanet.dev/


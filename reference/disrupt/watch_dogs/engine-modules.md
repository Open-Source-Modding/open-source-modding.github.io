# Engine Modules & Library Names

> **Source**: Watch_Dogs Series Modding Discord (wasd, the task of the Pesky Fly, VinceM, みる97, 4/16/19). These are the **internal codenames/libraries** of the Disrupt engine (Dunia 2 fork). Useful for spotting what a DLL/script/binary actually does.

## Core libraries

| Name | Purpose | Notes |
|------|---------|-------|
| **Pilot** | Pathfinding / navigation | "The Pilot navigates/pilots around the city (pathfinding)". `pilot/` data folder holds instructions for where pilots (NPCs) have to fly/drive. |
| **Storm** | Networking | "Storm has to handle a storm of packets coming at it from the net". |
| **Gears** | Utility serializer | "Some shitty utility lib. Used everywhere from Assassins Creed to WD and uplay itself" (VinceM). A "stl replacement" — same resource format across Ubisoft games. |
| **DARE** | Audio | "DARE screams at you through your speakers (audio)". |
| **depload** | Dependency preload manifest | `.depload.dat` files list hashed resource paths so the streaming system preloads before demand. See [depload-format.md](../depload-format.md). "To add resources you still have to go through the depload file" (みる97). |

## Game systems

| Name | Purpose | Notes |
|------|---------|-------|
| **APS\*** | Progression / achievements | Prefix on cut content: `APSCollectibles.ToBeDeleted_Feats45` ("whatever feats were, there was 45 of them"), `APSMainMissions.ToBeDeleted_A02_M04`, `A02_M05`, `A03_M02`. The `ToBeDeleted_` prefix marks **cut content** left in the data. The system says "SKILLS" — possible skill-unlock mechanism. |
| **CDebugRecorder** | Debug recording | Named in the same 2012-era data context. |

## Related facts

- In the 2012 demo, peds walked on bridges — removed because "there could be problems when the bridge was raised" (Pesky Fly).
- Same resource format across Ubisoft games is why Far Cry tools (Gibbed.Disrupt, FCBastard) work across Disrupt titles.
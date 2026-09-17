# Wwise Soundbanks in Disrupt

## Overview

Watch Dogs: Legion (and WD2) use Audiokinetic Wwise for audio. Sound data is stored in Wwise SoundBank (`.bnk`) files within the Disrupt archive system.

## WDL SoundBank Version

**WDL soundbanks = Wwise SoundBank version 132 (0x84)**

This corresponds to **Wwise 2019.2.x**.

## SoundBank Chunk Structure

WDL BNKs follow standard Wwise chunk layout:

```
BKHD (version 132) + DIDX + DATA + HIRC
```

- **BKHD**: Bank header (contains version, ID, etc.)
- **DIDX**: Data index (file offsets/sizes for embedded media)
- **DATA**: Audio data blob
- **HIRC**: Hierarchy (event/action/structure definitions)

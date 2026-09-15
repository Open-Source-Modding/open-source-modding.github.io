# WD1 XBG Import Tools (ResHax thread)

Source: ResHax forum thread "Watch Dogs 1 (PC)" (3D/2D models, Nov 2024–Jul 2025),
thread by andreagtacj, solved by shak-otay.

## Tools

- **Blender 2.49b XBG importer** — `Blender249[WatchDogs][PC][xbg][2014-10-25].zip`
  (66.54 kB), by **Mariusz Szkaradek** (Szkaradek123). Downloadable from the
  ResHax thread. All kudos go to Mariusz.
- andreagtacj (Nov 2024): had the toolset for WD1 ripping, including the 2.49
  Blender xbg importer, originally downloaded 2–3 years prior from XeNTaX.

## Requirements

- **Important:** for Windows 7 and higher, copy the `newGameLib` folder into
  the Blender **2.49b root directory**.
- The importer **does not work with newer Blender versions** — 2.49b only.

## Errors

- Loading an xbg gives an error that the folder `newGameLib` doesn't exist,
  even when it sits next to the .py file. Copying it into the Blender root
  fixes that.
- Running the script in 2.49's Text Editor produced `no module named
  genericpath` in the console (andreagtacj, Nov 2024) — an environment issue
  on newer Windows, not a file problem.
- shak-otay's `newGameLib/myLibraries` subfolder contained 9 py scripts
  (instead of 8 in the original) with 6 larger — potential mismatch issue.
- The old `newGameLib` works too; joints come out shorter.

## Notes

- shak-otay verified the script works: imported `char01-xbg.png` sample
  successfully (used `Watchdogs.py` from Mariusz Szkaradek, Oct 25 2014).
- DemonTimeKilla (Jul 8 2025) requested access to the WD1 ripping tools;
  shak-otay pointed to the attached Szkaradek123 importer zip.
---
title: "Far Cry 5/ND/6 dialog trigger ID mapping"
---

# Dialog trigger ID mapping (FC5 → ND → FC6)

FC5 stores dialog triggers by name (e.g. `trigger_reflex_jump`). ND and FC6
switched to numeric IDs. FCBConverter's markup XML conversion needs a lookup
table to map IDs back to names.

## How it works

In markup `.bin` files (cinematic triggers), dialog trigger events reference
triggers by ID:

```xml
<Frame Time="0.13346702" FrameCRC64="16045091570232110294">
  <object hash="5BCF3D91" name="CDialogTriggerEvent">
    <field hash="60534B32" name="hidEventName" value-String="DialogTriggerEvent" .../>
    <field hash="DCC35857" name="hidTargetEntityId" type="BinHex">FDC48DA466072000</field>
    ...
  </object>
</Frame>
```

The `hidTargetEntityId` field (`FDC48DA466072000` = decimal `9015336683554045`)
is the dialog trigger ID. To resolve it to a name, look it up in the trigger
list and replace the `DialogTrigger` object with a `text_sDialogTriggerName`
field:

```xml
<field hash="77586435" name="text_sDialogTriggerName"
       value-String="trigger_reflex_jump" type="BinHex">
  747269676765725F7265666C65785F6A756D7000
</field>
<field hash="FC7F79B1" name="sDialogTriggerName"
       value-ComputeHash32="trigger_reflex_jump" type="BinHex">
  7C9F092F
</field>
```

## Trigger ID differences between ND and FC6

Some triggers have different IDs in FC6 vs ND. If the markup file is from
FC6, use the FC6 ID; if from ND, use the ND ID.

**Known FC6-specific IDs** (different from ND):
- `9015346338701701` — Jump (FC6) vs `9015336683554045` (ND)
- `9015346338700914` — Ono_Effort_Light (FC6) vs different ND ID

## Conversion workflow (FC5 → ND/FC6)

1. Parse the markup XML to find all `CDialogTriggerEvent` objects
2. For each, extract `hidTargetEntityId` (the numeric ID)
3. Look up the ID in the trigger list to get the trigger name
4. Add `text_sDialogTriggerName` and `sDialogTriggerName` fields
5. Remove the `DialogTrigger` object
6. Repeat for all triggers in the file

## Source

legendhavoc175 + BIRDdude12, Discord Far Cry modding community, Dec 2025.

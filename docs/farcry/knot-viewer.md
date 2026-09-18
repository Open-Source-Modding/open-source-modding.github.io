---
title: "KnotViewer — FC animation curve editor"
---

# KnotViewer (KnotViewer.pyw)

Interactive tkinter/matplotlib editor for Far Cry knot XML data (animation
curves/splines). Located at `re/Ubisoft/Dunia/KnotViewer.pyw`.

## What it does

Pastes knot XML from FCBConverter's `.markup.bin.converted.xml` output, displays
the curves on a plot, and lets you drag knot positions and tangent handles.
Exports back to XML for re-injection.

## XML format

```xml
<object hash="F32C0E1C" name="Knots">
 <object hash="9A152447" name="Knot">
 <field hash="DCB67730" name="Value" value-Vector4="time,value,hx,hy"
 type="BinHex">...</field>
 <field hash="6BBB9E69" name="Info"
 value-Vector4="6.2831993,1,0,0" type="BinHex">...</field>
 <field hash="2CECF817" name="Type" value-Int32="0" type="BinHex">...</field>
 </object>
</object>
```

- `Value` Vector4: (time, value, handle_x, handle_y) — handle is relative to point
- `Info` Vector4: (frequency, amplitude, ?, ?)
- `Type` Int32: interpolation type

## Hash table

| Hash | Name |
|------|------|
| `F32C0E1C` | Knots container |
| `9A152447` | Knot |
| `DCB67730` | Value (Vector4) |
| `6BBB9E69` | Info (Vector4) |
| `2CECF817` | Type (Int32) |

## Usage

Requires Python 3 + matplotlib. Run `KnotViewer.pyw`. Paste knot XML into the
text box, click "load knots", edit on the plot (drag points or handles), then
"export knots" to get the modified XML back.

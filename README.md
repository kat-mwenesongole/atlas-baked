# Atlas Baked

A simple tool to generate font atlases from TrueType files and export glyph data in a custom `.font` format.

---

## Usage

### GUI
1. Place the executable anywhere (`/build/Atlas Baked (windows).exe`)  
2. Run  

### Command Line
1. Place the executable anywhere (`/build/Atlas Baked (windows).exe`)  
2. Run with arguments:  

- `-ttf` → Input TrueType font file  
- `-s` → Output file path  
- `-h` → Font height (in points)  

Example:
```
Atlas" "Baked" "^(windows^).exe -ttf"input.ttf" -s"output.font" -h"72"
```

---

## Output

- **`.font` file** – Contains glyph data (UVs, width, height, spacing, etc.)  
- **`.bmp` file** – Preview of the generated atlas (identical to the one stored in `.font`)

## `.font` File Layout

### `font_header`

| Field | Type | Description |
|---|---:|---|
| `size` | `s32` | Total size of the font data / header metadata field |
| `width` | `s32` | Width of the atlas texture in pixels |
| `height` | `s32` | Height of the atlas texture in pixels |
| `glyph_count` | `s32` | Number of glyphs stored in the file |
| `glyph_height` | `s32` | Default / nominal glyph height |
| `glyph_width` | `s32` | Default / nominal glyph width |
| `line_spacing` | `s32` | Vertical spacing between lines of text |
| `glyph_offset` | `s32` | Byte offset to the glyph header array |
| `byte_offset` | `s32` | Byte offset to the raw atlas bitmap data |
| `glyphs[GLYPH_COUNT]` | `glyph_header[233]` | Fixed-size array containing metadata for each glyph |

### `glyph_header`

| Field | Type | Description |
|---|---:|---|
| `character` | `s8` | Character / codepoint represented by this glyph |
| `offset` | `s32` | Byte offset to this glyph’s pixel data or related stored data |
| `spacing` | `s32` | Advance spacing after drawing the glyph |
| `pre_spacing` | `s32` | Spacing before drawing the glyph |
| `width` | `s32` | Glyph width in pixels |
| `height` | `s32` | Glyph height in pixels |
| `u0` | `r32` | Left texture coordinate |
| `u1` | `r32` | Right texture coordinate |
| `v0` | `r32` | Top texture coordinate |
| `v1` | `r32` | Bottom texture coordinate |

### Constants

| Name | Value | Description |
|---|---:|---|
| `GLYPH_COUNT` | `233` | Number of glyph entries stored in the font |
| `GLYPH_ROWS` | `16` | Number of rows in the atlas grid |
| `GLYPH_COLUMNS` | `16` | Number of columns in the atlas grid |

### Notes

- The file begins with a `font_header`.
- Glyph metadata is stored in the embedded `glyphs` array.
- `byte_offset` points to the atlas image data.
- UV coordinates (`u0`, `u1`, `v0`, `v1`) map each glyph into the atlas texture.

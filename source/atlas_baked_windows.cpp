// Tedious Typography
//
// Font sizes can be specified in points (pt), 1 pt = 1/72 inch (0.35 mm).
//
// The font size (height) is broken up into the ascent and descent.
// Ascent is the space above the baseline.
// Descent is the space below the baseline.
//
// Font size = Ascent + Descent
// e.g A 72pt font may have an ascent of 56pt and descent of 16pt, thus 72pt = (56pt + 16pt)
//
// Font sizes can also be specified in logical units,
// 1 logical unit = 72pt (1 inch, 2.54 cm).
// 1 logical unit = DPI (px) (historically 96 px, but can be set on a user by user basis)
// e.g DPI setting of 100% (96 px), DPI setting of 125% (120 px), DPI setting of 150% (144 px)
//
// Font sizes can also be specified in device independent pixels (DIPs), 1 DIP = 1/96 logical unit.
// Therefore,
// 1 inch = 72 pt = 1 logical unit = 96 px (DPI, 100%)  = 1 DIP
// 1 inch = 72 pt = 1 logical unit = 144 px (DPI, 150%) = 1.5 DIP
//
// to get the font size in pixels given points:
// pixels = (points/72)*96*(DPI/100)
// 
// Windows peculiarities
// Something to note about Windows 'SetProcessDpiAwareness()'.
// If a program does not set its DPI awareness, then Windows scales the window (among other things) when the users DPI setting is not set to 100%.
// e.g A DPI unaware program specifies a window size of 500x500 on a machine that has a DPI setting of 150%.
//     Windows will scale the window to 750x750 (DWM scaling).
//

//
// ---2160p
//
// 9. 2160px
// 8. 1920px
// 7. 1680px
// 6. -
// 5. 1200px
// 4. -
// 3. -
// 2. -
// 1. -
//
// ---1440p
//
// 9. 1440px
// 8. 1280px
// 7. 1120px
// 6. -
// 5. 800px
// 4. -
// 3. -
// 2. -
// 1. -
// 
// ---1080p
//
// 1080px
//
// 9. 1080px
// 8. 960px
// 7. 840px
// 6. - 
// 5. 600px
// 4. -
// 3. 360px
// 2.  -
// 1. 120px
//
// ---720p
//
// 9. 720px
// 8. 640px
// 7. 560px
// 6. 480px
// 5. 400px
// 4. 320px
// 3. 240px
// 2. 160px
// 1.  80px
//

// 
//
// 2160px =
// 1920px =
// 1680px =
// 1440px =
// 1280px =
// 1200px =
// 1120px =
// 1080px =
//  960px =
//  840px =
//  800px =
//  720px =
//  640px =
//  600px =
//  560px =
//  480px =
//  400px =
//  360px =
//  320px =
//  240px =
//  160px =
//  120px =
//   80px = 
//
//



#include <windows.h>
#include <shellscalingapi.h>
#include <shlwapi.h>

#include "p:/handmade/handmade.cpp"
#include "p:/handmade/handmade_os.cpp"
#include "p:/handmade/opengl/handmade_opengl.cpp"
#include "p:/handmade/opengl/handmade_opengl_windows.cpp"
#include "p:/handmade/windows/handmade_windows.cpp"
#include "p:/handmade/maths/handmade_math.cpp"
#include "p:/handmade/handmade_string.cpp"

#include <cstdio>

global b32 running;

global s8   open_file[MAX_PATH] = { };
global s8   save_file[MAX_PATH] = { };
global s8 bitmap_file[MAX_PATH] = { };

global s8 fontheight_field[3] = { };



#pragma pack(push, 1)

// bitmap.
struct bitmap_header
{
    u16   signature; // must be 'BM' (0x4d42)
    u32   file_size;
    u16  reserved_0; // must be 0
    u16  reserved_1; // must be 0
    u32 byte_offset; // offset into the file the actual pixel array begins (must be 122)

    u32 header_size; // sizeof(BITMAPINFOHEADER)
    s32       width;  
    s32      height; // positive (bottom-up DIB)
    
    u16 planes;              // must be 1
    u16 bits_per_pixel;      // must be 32    
    u32 compression;         // must be BI_BITFIELDS
    u32 image_size;          // must be 0
    s32 x_pixels_per_meter;  // must be 0 (no preference)
    s32 y_pixels_per_meter;  // must be 0 (no preference)
    u32        used_colours; // must be 0
    u32 significant_colours; // must be 0
};
// ttf.
#define TTF_SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define TTF_SWAPLONG(x) MAKELONG(TTF_SWAPWORD(HIWORD(x)), TTF_SWAPWORD(LOWORD(x)))
struct ttf_offsettable_header
{
    u16 major_version;
    u16 minor_version;
    u16 num_of_tables;
    u16 uSearchRange;
    u16 uEntrySelector;
    u16 uRangeShift;
};
struct ttf_directorytable_header
{
    s8 table_name[4]; 
    u32 checksum; 
    u32 offset; 
    u32 length; 
};
struct ttf_nametable_header
{
    u16 format_selector; // must be 0
    u16 namerecords_count; 
    u16 storage_offset;
};
struct ttf_name_header
{
    u16 platform_id;
    u16 encoding_id;
    u16 language_id;
    u16 name_id;
    u16 string_length;
    u16 string_offset;
};
#pragma pack(pop)

// // // // // // // // // // // FONT

#define HEADER_FONT_GLYPH_COUNT   233
#define HEADER_FONT_GLYPH_R 16
#define HEADER_FONT_GLYPH_C 16

#define    FONT_RESOLUTIONS_COUNT 23

global u32 FONT_RESOLUTIONS[FONT_RESOLUTIONS_COUNT] =
{
    80,
    120,
    160,
    240,
    320,
    360,
    400,
    480,
    560,
    600,
    640,
    720,
    800,
    840,
    960,
    1080,
    1120,
    1200,
    1280,
    1440,
    1680,
    1920,
    2160
};

#pragma pack(push, 1)
struct header_font_glyph
{
    s8  character;
    s32 offset;
    s32     spacing;
    s32 pre_spacing;
    s32  width;
    s32 height;
    r32 u0;
    r32 u1;
    r32 v0;
    r32 v1;
};
struct header_font
{
    s32   size;
    s32  width;
    s32 height;
    s32 glyph_count;
    s32 glyph_height;
    s32 glyph_width;
    s32 line_spacing;
    s32 glyph_offset;
    s32  byte_offset;
    
    header_font_glyph glyphs[HEADER_FONT_GLYPH_COUNT];
};
#pragma pack(pop)
// // // // // // // // // // // FONT


// bitmap.
internal void
bitmap_saveas(s8* bitmap_file, s32 bitmap_width, s32 bitmap_height, s8* bitmap_data)
{
    s32 bitmap_size = bitmap_width * bitmap_height * 4;

    bitmap_header header = {};
    header.signature      = 0x4D42; 
    header.file_size      = sizeof(bitmap_header) + bitmap_size;
    header.byte_offset    = sizeof(bitmap_header); 
    header.header_size    = sizeof(BITMAPINFOHEADER); 
    header.width          = bitmap_width;  
    header.height         = bitmap_height; 
    header.planes         = 1;            
    header.bits_per_pixel = 32;      
    header.compression    = BI_RGB;
    header.image_size     = bitmap_size;

    s8* save = (s8*)VirtualAlloc(0, header.file_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    mem_copy(&header, save, header.header_size);
    mem_copy(bitmap_data, save + header.byte_offset, bitmap_size);

    io_writefile(bitmap_file, header.file_size, save);

    VirtualFree(save, 0, MEM_RELEASE);
}
// ttf.
internal b32
ttf_fontfamily(s8* font_file, s8* font_family)
{
    io_file font = io_readfile(font_file);
    if(font.source)
    {
	ttf_offsettable_header* offset_table = (ttf_offsettable_header*)font.source;
	offset_table->num_of_tables = TTF_SWAPWORD(offset_table->num_of_tables);

	b32 table_found = false;

	ttf_directorytable_header* directory_table =
	(ttf_directorytable_header*)((s8*)font.source + sizeof(ttf_offsettable_header));
	for(s32 table = 0; table < offset_table->num_of_tables; table++)
	{
	    if(directory_table->table_name[0] == 'n' &&
	       directory_table->table_name[1] == 'a' &&
	       directory_table->table_name[2] == 'm' &&
	       directory_table->table_name[3] == 'e')
	    {
		table_found = true;
		directory_table->length = TTF_SWAPLONG(directory_table->length);
		directory_table->offset = TTF_SWAPLONG(directory_table->offset);
		break;
	    }
	    directory_table++;
	}

	if(table_found)
	{
	    ttf_nametable_header* name_table = (ttf_nametable_header*)((s8*)font.source + directory_table->offset);

	    name_table->namerecords_count = TTF_SWAPWORD(name_table->namerecords_count);
	    name_table->storage_offset    = TTF_SWAPWORD(name_table->storage_offset);

	    ttf_name_header* name_header = (ttf_name_header*)((s8*)name_table + sizeof(ttf_nametable_header));
	    for(s32 record = 0; record < name_table->namerecords_count; record++)
	    {
		name_header->name_id = TTF_SWAPWORD(name_header->name_id);
		if(name_header->name_id == 1) // font family
		{
		    name_header->string_length = TTF_SWAPWORD(name_header->string_length);
		    name_header->string_offset = TTF_SWAPWORD(name_header->string_offset);

		    // note, we can not to a simple 'mem_copy' as we require a null terminated string.
		    //       it would appear the spaces between letters are '\0'

		    s8* start = (s8*)font.source + directory_table->offset + name_table->storage_offset + 
		    name_header->string_offset + 1;
		    for(s32 c = 0; c < name_header->string_length - 1; c++)
		    {
			if(start[c] != '\0')
			{
			    *font_family++ = start[c];
			}
		    }
		    
		    io_freefile(font);
		    return(true);
		}
		name_header++;
	    }
	}
	
    }
    return(false);
}
// bake.
internal void
bake_writeglyph(void* source, u32 source_size_x, u32 source_size_y, u32 source_width,
		void* target, u32 target_width)
{
    for(u32 sy = 0; sy < source_size_y; sy++)
    {
	mem_copy(source, target, (source_size_x * 4));
	target = (u32*)target + target_width;
	source = (u32*)source + source_width;
    }
}
internal u32*
bake_loadglyph(HDC device_context, void* bytes,
	       s32 font_height, // in pixels.
	       s32* offset,
	       s32* glyph_width,
	       s32* glyph_height)
{
    ASSERT(bytes);

    // text extent?
    s32  subsection_width  = font_height * 2;
    s32  subsection_height = font_height * 2;
    u32* subsection        = (u32*)VirtualAlloc(0, (subsection_width * subsection_height * 4), MEM_COMMIT, PAGE_READWRITE);

    // copy and calculate bounds.
    s32 max_column = 0;
    s32 min_column = subsection_width;
    s32 max_row    = 0;
    s32 min_row    = subsection_height;

    u32* dib_memory = (u32*)bytes;
    u32* ptr = subsection;
    for(s32 y = 0; y < subsection_height; y++)  
    {
	u32* px = dib_memory;
	for(s32 x = 0; x < subsection_width; x++) 
	{
	    u8 a = *px++ & 0xff; 
	    if(a)
	    {
		if(x < min_column) min_column = x;
		if(x > max_column) max_column = x;
		if(y < min_row)    min_row    = y;
		if(y > max_row)    max_row    = y;
	    }
	    *ptr++ = a | (a << 8) | (a << 16) | (a << 24);
	}
	dib_memory += (font_height * 2);
    }
	    
    *glyph_width  = (max_column != 0) ? ((max_column - min_column) + 1) : 0;
    *glyph_height = (max_row    != 0) ? ((max_row    - min_row   ) + 1) : 0;

    // this is a save guard, shouldn't actually happen in practice! change to 'ASSERT'
    *glyph_width  = (*glyph_width  > font_height) ? font_height : *glyph_width;
    *glyph_height = (*glyph_height > font_height) ? font_height : *glyph_height;

    // smallest possible glyph.
    u32* glyph = (u32*)VirtualAlloc(0, *glyph_width * *glyph_height * 4, MEM_COMMIT, PAGE_READWRITE);
    if(glyph)
    {
	u32* subsection_ptr = subsection;
	subsection_ptr += (min_row * subsection_width) + min_column;

	bake_writeglyph(subsection_ptr, *glyph_width, *glyph_height, subsection_width, glyph, *glyph_width);
	
	VirtualFree(subsection, 0, MEM_RELEASE);
    }
    // free happens later.

    TEXTMETRICA metrics = {};
    GetTextMetricsA(device_context, &metrics);
    *offset = max_row - (subsection_height - metrics.tmAscent);

    return((u32*)glyph); 
}
/*
  internal b32
  bake_loadfont(font_header* atlas, s32 pt, s32 px, s8* font_file, u32** glyphs)
  {
  b32 success = false;

  AddFontResourceExA(font_file, FR_PRIVATE, 0);

  s8 font_family[256] = {};
  ttf_fontfamily(font_file, font_family);

  HFONT font_handle = CreateFontA(-MulDiv(pt, GetDeviceCaps(GetDC(0), LOGPIXELSY), 72), 0, 0, 0,
  FW_NORMAL,   // weight
  FALSE,       // italic
  FALSE,       // underline
  FALSE,       // strikeout
  DEFAULT_CHARSET, 
  OUT_DEFAULT_PRECIS,
  CLIP_DEFAULT_PRECIS, 
  ANTIALIASED_QUALITY,
  DEFAULT_PITCH | FF_DONTCARE,
  font_family);
    
  if(font_handle)
  {
  HDC device_context = CreateCompatibleDC(GetDC(0));
  if(device_context)
  {
  SetMapMode(device_context, MM_TEXT);
	    
  BITMAPINFO bitmap_info              = {};
  bitmap_info.bmiHeader.biSize        =  sizeof(bitmap_info.bmiHeader);
  bitmap_info.bmiHeader.biWidth       =  pixels * 2;
  bitmap_info.bmiHeader.biHeight      =  pixels * 2; // (+) bottom-up, (-) top-down
  bitmap_info.bmiHeader.biPlanes      =  1;
  bitmap_info.bmiHeader.biBitCount    =  32;
  bitmap_info.bmiHeader.biCompression =  BI_RGB;

  s32 max_offset = 0;
  s32 c = 0;
  for(s32 g = 32; g < 256; g++) // '!'(32) -> 'ÿ'(255) 
  {
  void*   bytes = 0;
  HBITMAP bitmap_handle = CreateDIBSection(device_context, &bitmap_info, DIB_RGB_COLORS, &bytes, 0, 0);
  if(bitmap_handle)
  {
  // standard.
		    
  SelectObject (device_context, bitmap_handle);
  SelectObject (device_context, font_handle);
  SetBkColor   (device_context, RGB(  0,   0,   0));
  SetTextColor (device_context, RGB(255, 255, 255));
  TextOutA(device_context, 0, 0, (LPCSTR)&g, 1); // character output.

  // ?
  // SIZE size;
  // GetTextExtentPoint32A(device_context, &character, 1, &size);
    
  glyphs[c] = bake_loadglyph(device_context, bytes, pixels,
  &atlas->glyphs[c].offset,
  &atlas->glyphs[c].width,
  &atlas->glyphs[c].height);
		    
  if(atlas->glyphs[c].offset > max_offset) { max_offset = atlas->glyphs[c].offset; }

  ABC character_metrics = {};
  GetCharABCWidthsA(device_context, (u32)g, (u32)g, &character_metrics);
  atlas->glyphs[c].character   = g;
  atlas->glyphs[c].    spacing = character_metrics.abcC;
  atlas->glyphs[c].pre_spacing = character_metrics.abcA;

  DeleteObject(bitmap_handle);
  }
  else
  {
  OutputDebugStringA("'CreateDIBSection' failed!\n");
  }
  c++;
  }
  for(u32 i = 0; i < GLYPH_COUNT; i++)
  {
  atlas->glyphs[i].offset = max_offset - atlas->glyphs[i].offset;
  }

  TEXTMETRIC metrics = {};
  GetTextMetrics(device_context, &metrics);
  atlas->line_spacing = metrics.tmInternalLeading;

  success = true;
  }
  else
  {
  OutputDebugStringA("'CreateCompatibleDC' failed!\n");
  }
  DeleteObject(font_handle);
  }
  else
  {
  OutputDebugStringA("'CreateFontA' failed!\n");
  }

  RemoveFontResourceExA(font_file, FR_PRIVATE, 0);

  return(success);
  };
  internal void
  bake_clearglyphs(u32** glyphs)
  {
  for(u32 glyph = 0; glyph < GLYPH_COUNT; glyph++)
  {
  VirtualFree(glyphs[glyph], 0, MEM_RELEASE);
  }
  }
  internal b32
  bake_font()
  {
  b32 success = false;

  r32 points = strtof(fontheight_field,0); 
  s32 pixels = (points/72)*96*(DPI/100);          
    
  u32* glyphs[GLYPH_COUNT] = {};
	 
  font_header* atlas = (font_header*)VirtualAlloc(0, sizeof(font_header) + (((pixels*GLYPH_COLUMNS)*(pixels*GLYPH_ROWS))*4), MEM_COMMIT, PAGE_READWRITE);
  atlas->glyph_width  = pixels;
  atlas->glyph_height = pixels;
  atlas->width        = atlas->glyph_width  * GLYPH_COLUMNS;
  atlas->height       = atlas->glyph_height * GLYPH_ROWS;
  atlas->size         = sizeof(font_header) + (atlas->width * atlas->height * 4);
  atlas->glyph_count  = GLYPH_COUNT;
  atlas->glyph_offset = 9 * sizeof(u32);
  atlas->byte_offset  = sizeof(font_header);

  // does the ttf file exist?
  if(PathFileExistsA(open_file))
  {
  if(bake_loadfont(atlas, points/2.0, pixels/2, open_file, glyphs))
  {
  s8* glyph_data = (s8*)atlas + atlas->byte_offset;
  for(s32 g = GLYPH_COUNT - 1; g > -1; g--)
  {
  u32 target_row    = g / GLYPH_COLUMNS;
  u32 target_column = g % GLYPH_COLUMNS;

  // the row height is equal to atlas->glyph_height.
		
  // bytes contained in a single row =
  // atlas->glyph_height * atlas->width * 4

  // bytes contained in a single glyph  row =
  // atlas->glyph_width * 4

  // glyph beginning row =
  // (target_row * 'bytes contained in a single row') + (target_column * 'bytes contained in a single glyph row')

  // bytes contianed in a single glyph =
  // atlas->glyph_width * atlas->glyph_height * 4

  // bytes contianed in entire glyph atlas =
  // 'bytes contianed in a single glyph' * GLYPH_ROWS * GLYPH_COLUMNS

  // points to the first row of bytes where the glyph_header should be placed. (bottom-up)
  s8* target =
  (glyph_data + (atlas->width * atlas->height * 4) - (atlas->width * atlas->glyph_height * 4))
  +
  (atlas->glyph_width * 4 * target_column)
  -
  (atlas->width * atlas->glyph_height * 4 * target_row);

  s8* source = (s8*)(glyphs[g]);
		
  bake_writeglyph(source, atlas->glyphs[g].width, atlas->glyphs[g].height, atlas->glyphs[g].width, target, atlas->width);

  // uv.
  atlas->glyphs[g].u0 = (target_column * atlas->glyph_width)/(r32)atlas->width;
  atlas->glyphs[g].v0 = ((((GLYPH_ROWS - 1) - target_row) * atlas->glyph_height) + atlas->glyphs[g].height)/(r32)atlas->height;
  atlas->glyphs[g].u1 = ((target_column * atlas->glyph_width) + atlas->glyphs[g].width)/(r32)atlas->width;
  atlas->glyphs[g].v1 = (((GLYPH_ROWS - 1) - target_row) * atlas->glyph_height)/(r32)atlas->height;
  }
	
  bake_clearglyphs(glyphs);

  // write font (.font)
  io_writefile(save_file, atlas->size, atlas);
  // write bitmap (.bmp)
  bitmap_saveas(bitmap_file, atlas->width, atlas->height, (s8*)atlas + atlas->byte_offset);

  //VirtualFree(tree.head, 0, MEM_RELEASE);
  VirtualFree(atlas, 0, MEM_RELEASE);

  success = true;
  }
  else
  {
      OutputDebugStringA("'windows_loadfont' failed!\n");
  }
}
  else
  {
      OutputDebugStringA("'windows_loadfont' failed!\n");

      // error: specified truetype file does not exist.
  }
    
return(success);
}
*/
internal void
ATLAS_GLYPH_FINDBOUNDS(u32* dib, u32* section,
		       s32  pixel_height,
		       s32  subsection_width,
		       s32  subsection_height,
		       u32* max_c, u32* min_c,
		       u32* max_r, u32* min_r)
{
    *max_c  = 0;
    *min_c  = subsection_width;
    *max_r  = 0;
    *min_r  = subsection_height;

    u32* dib_memory = dib;
    u32* ptr = section;
    for(s32 y = 0; y < subsection_height; y++)  
    {
	u32* px = dib_memory;
	for(s32 x = 0; x < subsection_width; x++) 
	{
	    u8 a = *px++ & 0xff; 
	    if(a)
	    {
		if(x < *min_c) *min_c = x;
		if(x > *max_c) *max_c = x;
		if(y < *min_r) *min_r    = y;
		if(y > *max_r) *max_r    = y;
	    }
	    *ptr++ = a | (a << 8) | (a << 16) | (a << 24);
	}
	dib_memory += (pixel_height * 2);
    }
}

internal void
ATLAS_GLYPH_MAKE(header_font_glyph* glyph_info, void* glyph_data, 
	            s32 pixel_height, u32 atlas_width, u32* max_offset,
		 HFONT* font_handle, HDC* device_context, BITMAPINFO* bitmap_info)
{
    void* bytes = 0;
    HBITMAP bitmap_handle = CreateDIBSection(*device_context, bitmap_info, DIB_RGB_COLORS, &bytes, 0, 0);
    if(bitmap_handle && bytes)
    {
	// CONFIG
	
	SelectObject (*device_context, bitmap_handle);
	SelectObject (*device_context, *font_handle);
	SetBkColor   (*device_context, RGB(  0,   0,   0));
	SetTextColor (*device_context, RGB(255, 255, 255));

	// DRAW GLYPH IN DIB
	
	TextOutA(*device_context, 0, 0, (LPCSTR)&glyph_info->character, 1);

	// EXTRACT GLYPH FROM DIB (FIND BOUNDS OF THE GLYPH)

	s32  subsection_width  = pixel_height*2;
	s32  subsection_height = pixel_height*2;
	u32* subsection        = (u32*)VirtualAlloc(0, (subsection_width * subsection_height * 4), MEM_COMMIT, PAGE_READWRITE);

	u32 max_c = 0;
	u32 min_c = 0;
	u32 max_r = 0;
	u32 min_r = 0;
	
	ATLAS_GLYPH_FINDBOUNDS((u32*)bytes, subsection, pixel_height, subsection_width, subsection_height,
	                       &max_c, &min_c,
			       &max_r, &min_r);

	glyph_info->width  = (max_c != 0) ? ((max_c - min_c) + 1) : 0;
	glyph_info->height = (max_r != 0) ? ((max_r - min_r) + 1) : 0;

	// WRITE GLYPH INTO SLOT

	u32* target = (u32*)glyph_data;
	u32* source = subsection + (min_r * subsection_width) + min_c;
	
	for(u32 sy = 0; sy < glyph_info->height; sy++)
	{
	    CopyMemory(target, source, (glyph_info->width * 4));
	    target += atlas_width;
	    source += subsection_width;
	}

	VirtualFree(subsection, 0, MEM_RELEASE);
	
	// GLYPH INFO
	
	TEXTMETRICA metrics = {};
	GetTextMetricsA(*device_context, &metrics);
	glyph_info->offset = max_r - (subsection_height - metrics.tmAscent);

	ABC character_metrics = {};
	GetCharABCWidthsA(*device_context, (u32)glyph_info->character, (u32)glyph_info->character, &character_metrics);
	
	glyph_info->    spacing = character_metrics.abcC;
	glyph_info->pre_spacing = character_metrics.abcA;

	// DELETE

	DeleteObject(bitmap_handle);
    }
    else
    {
	OutputDebugStringA("'CreateDIBSection' failed!\n");
    }
}
internal header_font*
ATLAS_FONT_MAKE(s8* TTF_FILE, s8* FONT_NAME, u64* atlas_size, u32 px, u32 pt)
{
    header_font* header = 0;
    
    AddFontResourceExA(TTF_FILE, FR_PRIVATE, 0);

    HFONT font_handle = CreateFontA(-MulDiv(pt, GetDeviceCaps(GetDC(0), LOGPIXELSY), 72), 0, 0, 0,
				    FW_NORMAL,   // weight
				    FALSE,       // italic
				    FALSE,       // underline
				    FALSE,       // strikeout
				    DEFAULT_CHARSET, 
				    OUT_DEFAULT_PRECIS,
				    CLIP_DEFAULT_PRECIS, 
				    ANTIALIASED_QUALITY,
				    DEFAULT_PITCH | FF_DONTCARE,
				    FONT_NAME);
    if(font_handle)
    {
	HDC device_context = CreateCompatibleDC(GetDC(0));

	if(device_context)
	{
	    SetMapMode(device_context, MM_TEXT);

	    BITMAPINFO bitmap_info              = {};
	    bitmap_info.bmiHeader.biSize        =  sizeof(bitmap_info.bmiHeader);
	    bitmap_info.bmiHeader.biWidth       =  px*2;
	    bitmap_info.bmiHeader.biHeight      =  px*2; // (+) bottom-up, (-) top-down
	    bitmap_info.bmiHeader.biPlanes      =  1;
	    bitmap_info.bmiHeader.biBitCount    =  32;
	    bitmap_info.bmiHeader.biCompression =  BI_RGB;

	    // CREATE FILE FORMAT

	    *atlas_size = sizeof(header_font) + ((px*HEADER_FONT_GLYPH_C) * (px*HEADER_FONT_GLYPH_R) * 4);
	    
	    header = (header_font*)VirtualAlloc(0, *atlas_size, MEM_COMMIT, PAGE_READWRITE);

	    header->glyph_width  = px; // max_glyphwidth
	    header->glyph_height = px; // max_glyphheight
	    header->width        = header->glyph_width * HEADER_FONT_GLYPH_C;
	    header->height       = header->glyph_height * HEADER_FONT_GLYPH_R;
	    header->size         = *atlas_size;
	    header->glyph_count  = HEADER_FONT_GLYPH_COUNT;
	    header->glyph_offset = 9 * sizeof(u32);
	    header->byte_offset  = sizeof(header_font);
	    
	    TEXTMETRIC metrics = {};
	    GetTextMetrics(device_context, &metrics);
	    header->line_spacing = metrics.tmInternalLeading;

	    u32 max_offset = 0;
	    
	    s8* bytes = (s8*)header + header->byte_offset;

	    for(u32 character = 32; character < 256; character++) // ' '(32) -> 'ÿ'(255)
	    {
		header_font_glyph* glyph_info = &header->glyphs[character-32];
		glyph_info->character = character;

		u32 target_r = character / HEADER_FONT_GLYPH_R;
		u32 target_c = character % HEADER_FONT_GLYPH_C;

		// FIND GLYPH BITMAP POSITION
		
		s8* glyph_data =
		(bytes + (header->width * header->height * 4) - (header->width * header->glyph_height * 4))
		+
		(header->glyph_width * 4 * target_c)
		-
		(header->width * header->glyph_height * 4 * target_r);

		// WRITE GLYPH TO BITMAP

		ATLAS_GLYPH_MAKE(glyph_info, glyph_data, 
			header->glyph_height, 
			header->width, &max_offset,
				 &font_handle, &device_context, &bitmap_info);

		// SET GLYPH INFO (UV COORDINATES)

		glyph_info->u0 = (target_c * header->glyph_width)/(r32)header->width;
		glyph_info->v0 = ((((HEADER_FONT_GLYPH_R - 1) - target_r) * header->glyph_height) + glyph_info->height)/(r32)header->height;
		glyph_info->u1 = ((HEADER_FONT_GLYPH_C * header->glyph_width) + glyph_info->width)/(r32)header->width;
		glyph_info->v1 = (((HEADER_FONT_GLYPH_R - 1) - target_r) * header->glyph_height)/(r32)header->height;

		if(glyph_info->offset > max_offset)
		{
		    max_offset = glyph_info->offset;
		}
	    }

	    for(u32 i = 0; i < HEADER_FONT_GLYPH_COUNT; i++)
	    {
		header->glyphs[i].offset = max_offset - header->glyphs[i].offset;
	    }

	}
	else
	{
	    OutputDebugStringA("'CreateCompatibleDC' failed!\n");
	}
	
	DeleteObject(font_handle);
    }
    else
    {
	OutputDebugStringA("'CreateFontA' failed!\n");
    }
    
    RemoveFontResourceExA(TTF_FILE, FR_PRIVATE, 0);

    return(header);
}
internal void*
ATLAS_BMP_MAKE(u64* atlas_bmp_size, header_font* font)
{
    bitmap_header* header = 0;
    
    *atlas_bmp_size = sizeof(bitmap_header) + (font->width * font->height * 4);
    
    header = (bitmap_header*)VirtualAlloc(0, *atlas_bmp_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(header)
    {
	header->signature      = 0x4D42; 
	header->file_size      = *atlas_bmp_size;
	header->byte_offset    = sizeof(bitmap_header); 
	header->header_size    = sizeof(BITMAPINFOHEADER); 
	header->width          = font->width;  
	header->height         = font->height; 
	header->planes         = 1;            
	header->bits_per_pixel = 32;      
	header->compression    = BI_RGB;
	header->image_size     = (font->width * font->height * 4);

	CopyMemory((s8*)header + header->byte_offset, (s8*)font + font->byte_offset, header->image_size);
    }

    return(header);
}

internal b32
ATLAS_TTF_EXTRACTNAME(s8* FONT_FILE,
		      s8* FONT_NAME, u32* FONT_NAME_SIZE)
{
    b32 success = true;

    io_file font = io_readfile(FONT_FILE);
    if(font.source)
    {
	ttf_offsettable_header* offset_table = (ttf_offsettable_header*)font.source;
	offset_table->num_of_tables = TTF_SWAPWORD(offset_table->num_of_tables);

	b32 table_found = false;

	ttf_directorytable_header* directory_table =
	(ttf_directorytable_header*)((s8*)font.source + sizeof(ttf_offsettable_header));
	for(s32 table = 0; table < offset_table->num_of_tables; table++)
	{
	    if(directory_table->table_name[0] == 'n' &&
	       directory_table->table_name[1] == 'a' &&
	       directory_table->table_name[2] == 'm' &&
	       directory_table->table_name[3] == 'e')
	    {
		table_found = true;
		directory_table->length = TTF_SWAPLONG(directory_table->length);
		directory_table->offset = TTF_SWAPLONG(directory_table->offset);
		break;
	    }
	    directory_table++;
	}

	if(table_found)
	{
	    ttf_nametable_header* name_table = (ttf_nametable_header*)((s8*)font.source + directory_table->offset);

	    name_table->namerecords_count = TTF_SWAPWORD(name_table->namerecords_count);
	    name_table->storage_offset    = TTF_SWAPWORD(name_table->storage_offset);

	    ttf_name_header* name_header = (ttf_name_header*)((s8*)name_table + sizeof(ttf_nametable_header));
	    for(s32 record = 0; record < name_table->namerecords_count; record++)
	    {
		name_header->name_id = TTF_SWAPWORD(name_header->name_id);
		if(name_header->name_id == 1) // font family
		{
		    name_header->string_length = TTF_SWAPWORD(name_header->string_length);
		    name_header->string_offset = TTF_SWAPWORD(name_header->string_offset);

		    // note, we can not to a simple 'mem_copy' as we require a null terminated string.
		    //       it would appear the spaces between letters are '\0'

		    s8* start = (s8*)font.source + directory_table->offset + name_table->storage_offset + name_header->string_offset;
		    for(s32 c = 0; c < name_header->string_length; c++)
		    {
			if(start[c] != '\0')
			{
			    *FONT_NAME++ = start[c];
			    (*FONT_NAME_SIZE)++;
			}
		    }
		    
		    io_freefile(font);
		    return(true);
		}
		name_header++;
	    }
	}
	
    }

    return(success);
}

s32 WINAPI
WinMain (HINSTANCE          instance,
	 HINSTANCE previous_instance,
	 LPSTR     commandline,
	 s32       show_commandline)
{
    s8 TTF_FILE[] = "..\\sample\\amestextcondensed.ttf";

    // CONFIG
    
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    u32 DPI = GetDpiForSystem();

    // EXTRACT FONT NAME AND FAMILY DIRECTLY FROM TTF_FILE

    s8 FONT_NAME[MAX_PATH]={}; // FONTNAME\0
    u32 FONT_NAME_SIZE = 0;

    if(ATLAS_TTF_EXTRACTNAME(TTF_FILE, FONT_NAME, &FONT_NAME_SIZE))
    {	
	for(u32 n = 0; n < FONT_RESOLUTIONS_COUNT; n++)
	{
	    u32 PX_HEIGHT = FONT_RESOLUTIONS[n];
	    u32 PT_HEIGHT = (72*PX_HEIGHT)/(96*(DPI/(r32)100));

	    s8 CUSTOM_NAME[MAX_PATH] = {};
	    CopyMemory(CUSTOM_NAME, FONT_NAME, FONT_NAME_SIZE); // FONTNAME

	    //
	    // PX_HEIGHT = 2160
	    //
	    // 2160 / 1000     = 2
	    // 2160 - (2*1000) = 160
	    // 160 / 100       = 1
	    // 160 - (1*100)   = 60
	    // 60 / 10         = 6
	    // 60 - (6*10)     = 0
	    //
	    // CHAR_PXHEIGHT[0] = '0' + 2   
	    // CHAR_PXHEIGHT[1] = '0' + 1 
	    // CHAR_PXHEIGHT[2] = '0' + 6 
	    // CHAR_PXHEIGHT[3] = '0' + 0
	    //
	    // CHAR_PXHEIGHT[] = "2160";
	    //

	    s8 DIGITS[4] = {};
	    DIGITS[0] = PX_HEIGHT/1000;
	    DIGITS[1] = (PX_HEIGHT - (1000*DIGITS[0]))/100;
	    DIGITS[2] = (PX_HEIGHT - (1000*DIGITS[0]) - (100*DIGITS[1]))/10;
	    DIGITS[3] = PX_HEIGHT - (1000*DIGITS[0]) - (100*DIGITS[1]) - (10*DIGITS[2]);

	    s8 CHAR_PXHEIGHT[5] = {};
	    CHAR_PXHEIGHT[0] = '_';
	    CHAR_PXHEIGHT[1] = '0' + DIGITS[0];
	    CHAR_PXHEIGHT[2] = '0' + DIGITS[1];
	    CHAR_PXHEIGHT[3] = '0' + DIGITS[2];
	    CHAR_PXHEIGHT[4] = '0' + DIGITS[3];
	    
	    CopyMemory((s8*)CUSTOM_NAME + FONT_NAME_SIZE, CHAR_PXHEIGHT, 5); // FONTNAME_XXXX

	    u64   atlas_size = 0;
	    void* atlas = ATLAS_FONT_MAKE(TTF_FILE, FONT_NAME, &atlas_size, PX_HEIGHT, PT_HEIGHT);

	    u64 atlas_bmp_size = 0;
	    void* atlas_bmp = ATLAS_BMP_MAKE(&atlas_bmp_size, (header_font*)atlas);

	    // WRITE .FONT

	    s8 ATLAS_PATH[MAX_PATH] = {};
	    CopyMemory(ATLAS_PATH, "..\\build\\", sizeof("..\\build\\")-1); // ../build/
	    CopyMemory(ATLAS_PATH + sizeof("..\\build\\")-1, CUSTOM_NAME, FONT_NAME_SIZE+5); // ../build/FONTNAME_XXXX
	    CopyMemory(ATLAS_PATH + sizeof("..\\build\\")-1 + (FONT_NAME_SIZE+5), ".font", sizeof(".kfont")); // ../build/FONTNAME_XXXX.font
	    
	    io_writefile(ATLAS_PATH, atlas_size, atlas);

	    VirtualFree(atlas, 0, MEM_RELEASE);

	    // WRITE .BMP

	    s8 ATLAS_BMP_PATH[MAX_PATH] = {};

	    CopyMemory(ATLAS_BMP_PATH, "..\\build\\", sizeof("..\\build\\")-1); // ../build/
	    CopyMemory(ATLAS_BMP_PATH + sizeof("..\\build\\")-1, CUSTOM_NAME, FONT_NAME_SIZE+5); // ../build/FONTNAME_XXXX
	    CopyMemory(ATLAS_BMP_PATH + sizeof("..\\build\\")-1 + (FONT_NAME_SIZE+5), ".bmp", sizeof(".bmp")); // ../build/FONTNAME_XXXX.bmp
	    
	    io_writefile(ATLAS_BMP_PATH, atlas_bmp_size, atlas_bmp);

	    VirtualFree(atlas_bmp, 0, MEM_RELEASE);
	}
    }
    else
    {
	OutputDebugStringA("'ATLAS_TTF_EXTRACTNAME' failed!\n");
    }

    return(0);
}

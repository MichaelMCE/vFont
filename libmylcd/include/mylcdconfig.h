
// libmylcd - http://mylcd.sourceforge.net/
// An LCD framebuffer and text rendering API
// Michael McElligott
// okio@users.sourceforge.net

//  Copyright (c) 2005-2011  Michael McElligott
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU LIBRARY GENERAL PUBLIC LICENSE
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU LIBRARY GENERAL PUBLIC LICENSE for more details.
//
//	You should have received a copy of the GNU Library General Public
//	License along with this library; if not, write to the Free
//	Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef _MYLCDCONFIG_H_
#define _MYLCDCONFIG_H_


// use pthreads for synchronization
// pthreadGC2.dll is required if enabled and is dynamically loaded
// 0:use the MS Windows API, 1:use pthreads
#define __BUILD_PTHREADS_SUPPORT__		0

// serialize access to the library
#define __BUILD_APISYNC_SUPPORT__		0

// use exports for the pixel accessor methods in places of defines
// ie; frame->pops->set(frame, ...) becomes lSetPixel(frame, ...)
#define __BUILD_PIXELPRIMITIVEEXPORTS__ 0

// '1' enables HTML character and entity reference support
#define __BUILD_NUM_ENTITYREF_SUPPORT__ 1
#define __BUILD_CHR_ENTITYREF_SUPPORT__ 1

// enable/disable drawing API
#define __BUILD_DRAW_SUPPORT__			1

// enable/disable rotate API
#define __BUILD_ROTATE_SUPPORT__		1

// enable/disable scroll API
#define __BUILD_SCROLL_SUPPORT__		1

// enable/disable support for PNG images
#define __BUILD_PNG_READ_SUPPORT__		1
#define __BUILD_PNG_WRITE_SUPPORT__		1

// enable/disable support for JPEG images
#define __BUILD_JPG_READ_SUPPORT__		1
#define __BUILD_JPG_WRITE_SUPPORT__		0

// enable/disable support for BMP images
#define __BUILD_BMP_SUPPORT__			1

// enable/disable support for TGA images
// (supports TARGA RLE read and write)
#define __BUILD_TGA_SUPPORT__			1

// enable/disable support for PGM images
// limited to 1bpp images and frame1
#define __BUILD_PGM_SUPPORT__			1

// enable write only support for GIF
// first frame only
#define __BUILD_GIF_SUPPORT__			1

// support for loading .ico and extraction of icons from .exe
#define __BUILD_ICO_SUPPORT__			1

// enable/disable support for BDF font
#define __BUILD_BDF_FONT_SUPPORT__		1

// compile support for image fonts (tga, bmp, etc..)"
#define __BUILD_BITMAP_FONT_SUPPORT__	1

// enable/disable print API
#define __BUILD_PRINT_SUPPORT__			1

// enable/disable internal font handling
// may require __BUILD_CHRDECODE_SUPPORT__
// requires either __BUILD_BITMAP_FONT_SUPPORT__
// or __BUILD_BDF_FONT_SUPPORT__
#define __BUILD_FONTS_SUPPORT__			1

// enable internal character decoding
#define __BUILD_CHRDECODE_SUPPORT__		1

// enable/disable internal Big5 lookup table
// 1:use an internal lookup table, 0:use map file CP950
#define __BUILD_INTERNAL_BIG5__			0

// enable/disable support for EUC-JP, ISO-2022-JP and Shift JIS decoding
// (EUC and the 7bit Japanese code points are promoted to Shift_JIS.)
#define __BUILD_INTERNAL_JISX0213__		0

// enable/disable support for HZ GB2312.80 decoding.
#define __BUILD_INTERNAL_HZGB2312__		0

// disable colour depths but 24/32 bpp
#define __BUILD_2432BITONLY_SUPPORT__	0


#define __BUILD_NULLDISPLAY__			0		// Template
#define __BUILD_NULLPORT__				1		// Template. Use with displays that no not require a port driver (DDRAW, SDL, G15, etc..)
#define __BUILD_FTDI__					0		// Access FTxxx devices through libftdi and libusb. Tested with FT245BM.
#define __BUILD_KS0108__				0
#define __BUILD_T6963C__				0
#define __BUILD_SED1335__				0		// S1D133x
#define __BUILD_SED1565__				0		// Found in Nokia 7110 (96x65)
#define __BUILD_PCD8544__				0		// Found in Nokia 3210, 3310, 5210, 8210 and others (usually 84x48)
#define __BUILD_PCF8814__				0		// Found in Nokia 3510, 6310, 5310 and others (usually 96x65)
#define __BUILD_PCF8833__				0		// Colour Nokia 6100, 6610, 7210, etc.. (usually 130x130)
#define __BUILD_S1D15G10__				0		// S1D15G00/10. Colour Nokia 6100, 6610, 7210, etc.. (usually 130x130)
#define __BUILD_S1D15G14__				0		// Found in Nokia 3510i (98x67)
#define __BUILD_G15LIBUSB__				0		// G15 accessed through libusb and the included G15 libusb0 driver (\mylcd\src\g15_libusb\g15_libusb_driver\)
#define __BUILD_USBD480LIBUSB__			0		// USBD480 through libusb0 (non HID firmware). HI-Speed USB display interface controller (www.lcdinfo.com) for the Sharp LQ043 LCD (480x272)
#define __BUILD_USB13700EXP__			0		// USB13700 expansion port configured for SPI. port0:control:0x378+2, port1:SPI:0x378 (requires __BUILD_USB13700LIBUSB__ or __BUILD_USB13700DLL__)
#define __BUILD_USB13700LIBUSB__		0		// S1D13700/USB13700 accessed through libusb (display_lib_USB13700.dll not required)

#ifdef __WIN32__
#define __BUILD_USB13700DLL__			0		// S1D13700/USB13700 accessed through display_lib_USB13700.dll (is dynamically loaded)
#define __BUILD_USBD480DLL__			0		// USBD480 through USBD480_lib.dll. HI-Speed USB display controller (www.lcdinfo.com) for the Sharp LQ043 LCD (480x272)
#define __BUILD_USBD480__				0		// USBD480 through libusb0 and HID (composite interface, if0).
#define __BUILD_G15DISPLAY__			0		// requires Logitech driver software (tested with v1.02/04). Links to lglcd.lib
#define __BUILD_G19DISPLAY__			0		// requires Logitech v3.02 driver or later. Links to lglcd.lib
#define __BUILD_SBUI__					1		// Interface to SwitchBlade UI (Razer)
#define __BUILD_DDRAW__					1		// Virtual display using DirectDraw
#define __BUILD_SDL__					0		// Virtual display using SDL. Requires SDL.dll
#define __BUILD_OPENGL__				0		// Virtual display using OpenGL
#define __BUILD_FT245__					0		// requires FTD2XX.dll (is dynamically loaded)
#define __BUILD_WINIO__					0		// requires winio.sys
#define __BUILD_OPENPARPORT__			0
#define __BUILD_LEDCARD__				0
#define __BUILD_SERIAL__				0
#define __BUILD_DLPORTIO__				0		// requires DLPORTIO.dll (is dynamically loaded)

#else
#define __BUILD_DDRAW__					0
#define __BUILD_LEDCARD__				0
#define __BUILD_WINIO__					0
#define __BUILD_OPENPARPORT__			0
#define __BUILD_DLPORTIO__				0
#define __BUILD_SERIAL__				0
#define __BUILD_FT245__					0
#define __BUILD_G15DISPLAY__			0
#define __BUILD_OPENGL__				1
#define __BUILD_SDL__					1
#endif

#define __BUILD_JPG_SUPPORT__ (__BUILD_JPG_READ_SUPPORT__||__BUILD_JPG_WRITE_SUPPORT__)
#define __BUILD_PNG_SUPPORT__ (__BUILD_PNG_READ_SUPPORT__||__BUILD_PNG_WRITE_SUPPORT__)

#define MaxPath							256

// character to use in place of tab (0x09)
#define lTabSpaceChar					32		// a Space

// number of lTabSpaceChar's per tab replacement
#define lTabSpaceWidth					4

// default line spacing in pixels per wrapped line
// can be negative
#define lLineSpaceHeight				2

#define MINFRAMESIZEW					2
#define MINFRAMESIZEH					MINFRAMESIZEW
#define MAXFRAMESIZEW					131072
#define MAXFRAMESIZEH					MAXFRAMESIZEW

#define lMaxDriverNameLength			127
#define lMaxDriverCommentLength			127


#ifdef __WIN32__
# define __BUILD_WIN32__ 1
#
# if 1
#  define MYLCD_APICALL __cdecl
# else
#  define MYLCD_APICALL __stdcall
# endif
#
# /* Set the default search path for the fonts and map files */
# /* Paths can also be passed through lOpen() */
# /* eg; lOpen(L"path/to/font/directories/", L"path/to/mapfiles"); */
# define lFontPath						L"../fonts/"
# define lCharacterMapPath				L"../mappings/"
#
//#ifdef BUILD_LIB
# define MYLCD_EXPORT __declspec(dllexport) MYLCD_APICALL
//#else
//# define MYLCD_EXPORT __declspec(dllimport) MYLCD_APICALL
//#endif
#
#else
# define MYLCD_EXPORT
#endif

#ifdef __LINUX__
# define __BUILD_LINUXPORT__ 1
#
# define lFontPath						L"/usr/share/libmylcd/fonts/"
# define lCharacterMapPath				L"/usr/share/libmylcd/mappings/"
#endif


#ifndef INLINE
# ifndef __DEBUG__
#  if defined(__GNUC__)
#   define INLINE __inline__
#  else
#   define INLINE inline
#  endif
# else
#  undef INLINE
#  define INLINE
# endif
#endif


#ifdef __DEBUG__
# define mylogw my_wprintf
# define mylog my_printf
#else
# define mylog(X,...)
# define mylogw(X,...)
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <ctype.h>
#include <wchar.h>
#include <wtypes.h> 

#endif



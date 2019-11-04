
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

#ifndef _MYLCD_H_
#define _MYLCD_H_

#include "mylcdconfig.h"


#define libmylcdVERSIONmj	0.80
#define libmylcdVERSIONmi	7
#define libmylcdVERSION		"0.80.7"
#define mySELF				"Michael McElligott"

// draw pixel styles
#define LSP_CLEAR			0
#define LSP_SET				1
#define LSP_XOR				2
#define LSP_OR				3
#define LSP_AND				4

// frame size
#define LFRM_BPP_1			0		// 8 pixels per byte
#define LFRM_BPP_8			1		// 8bit RGB332. 1 byte per pixel
#define LFRM_BPP_12			2		// 12bit RGB444. 2 bytes per pixel, 4 unused bits
#define LFRM_BPP_15			3		// 16bit RGB555. 2 bytes per pixel, 1 unused bit
#define LFRM_BPP_16			4		// 16bit RGB565. 2 bytes per pixel
#define LFRM_BPP_24			5		// 24bit RGB888. 3 bytes per pixel
#define LFRM_BPP_32			6		// 32bit RGBA888 with no alpha. 4 bytes per pixel
#define LFRM_BPP_32A		7		// 32bit+Alpha RGBA8888. 4 bytes per pixel
#define LFRM_TYPES			(LFRM_BPP_32A+1)	// number of BPP formats available

// use with lGetRGBMask()
#define LMASK_RED			0
#define LMASK_GREEN			1
#define LMASK_BLUE			2
#define LMASK_WHITE			3
#define LMASK_BLACK			4
#define LMASK_MAGENTA		5
#define LMASK_YELLOW		6
#define LMASK_CYAN			7

// copy modes for lCopyAreaScaled(), lCopyFrame() and lCopyAreaEx()
#define LCASS_CLEAR			0
#define LCASS_CPY			1
#define LCASS_XOR			2
#define LCASS_OR			3
#define LCASS_AND			4
#define LCASS_NXOR			5
#define LCASS_NOT			6

// text rendering mode
#define LPRT_CLEAR			LCASS_CLEAR
#define LPRT_CPY			LCASS_CPY
#define LPRT_OR				LCASS_OR
#define LPRT_AND			LCASS_AND
#define LPRT_XOR			LCASS_XOR
#define LPRT_NOT			LCASS_NOT

// glyph rendering effects, some modes are unsuitable with certain fonts
enum _TRFILTER
{
	LTR_0,
	LTR_180,
	LTR_FLIPH,
	LTR_FLIPV,
	LTR_OUTLINE1,			// 4 point outline
	LTR_OUTLINE2,			// 8 point outline
	LTR_OUTLINE3,			// 8 point outline without core
	LTR_SMOOTH1,			// 2 point smoothing
	LTR_SMOOTH2,			// 4 point smoothing
	LTR_SKEWEDFW,			// skewed forward
	LTR_SKEWEDFWSM1,		// skewed forward with 2 point smoothing
	LTR_SKEWEDFWSM2,		// skewed forward with 4 point smoothing
	LTR_SKEWEDBK,			// skewed backward
	LTR_SKEWEDBKSM1,		// skewed backward with 2 point smoothing
	LTR_SKEWEDBKSM2,		// skewed backward with 4 point smoothing
	LTR_SHADOW,				// north, south, east and/or west shadow
	LTR_BLUR1,
	LTR_BLUR2,
	LTR_BLUR3,
	LTR_BLUR4,
	LTR_BLUR5,
	LTR_BLUR6,
	LTR_BLUR7,
	LTR_BLUR8,
	LTR_USER1,				// can be used when overriding render method
	LTR_USER2,
	LTR_TOTAL,
	LTR_NONE	=	LTR_0,
	LTR_DEFAULT	=	LTR_NONE
};

// Blur filter attribute flags
#define LTRA_BLUR_COLOUR	0			// blur colour (24bit)
#define LTRA_BLUR_RADIUS	1			// blur radius
#define LTRA_BLUR_SETTOP	2			// 1:render top layer over underlay, 0:underlay only
#define LTRA_BLUR_X			3			// underlay x position offset
#define LTRA_BLUR_Y			4			// underlay y position offset
#define LTRA_BLUR_ALPHA		5			// underlay alpha


// Shadow filter attribute 0 flags
#define LTRA_SHADOW_N		(1<<31)			/* direction */
#define LTRA_SHADOW_S		(1<<30)
#define LTRA_SHADOW_W		(1<<29)
#define LTRA_SHADOW_E		(1<<28)
#define LTRA_SHADOW_S1		(1<<24)			/* thickness */
#define LTRA_SHADOW_S2		(1<<23)
#define LTRA_SHADOW_S3		(1<<22)
#define LTRA_SHADOW_S4		(1<<21)
#define LTRA_SHADOW_S5		(1<<20)
#define LTRA_SHADOW_OS(n)	(((n)&0xFF)<<8)	/* offset (0-255) */
#define LTRA_SHADOW_TR(n)	((n)&0xFF)		/* transparency (0-255) */

// Shadow filter attribute 1 flags
#define LTRA_SHADOW_BKCOL(n)	((n)&0xFFFFFF)

// Blur methods for lBlurImage()
#define lBLUR_HUHTANEN		1
#define lBLUR_STACKFAST		2
#define lBLUR_GAUSSIAN		3

// lDrawMask mode
#define LMASK_CLEAR			0
#define LMASK_OR			1
#define LMASK_XOR			2
#define LMASK_AND			3
#define LMASK_CPYSRC		4
#define LMASK_CPYMASK		5

// lMoveArea mode
#define LMOV_CLEAR			0		// clear all
#define LMOV_SET			1		// set all
#define LMOV_LOOP			2		// loop back to beginning
#define LMOV_BIN			3		// dump pixels

#define LMOV_LEFT			1
#define LMOV_RIGHT			2
#define LMOV_UP				3
#define LMOV_DOWN			4

// scroll flags
#define SCR_LOOP			0x01	// set scroller to loop back mode.
#define SCR_AUTOREFRESH		0x02	// set scroller to auto lRefresh() directly on each lUpdateScroll() call
#define SCR_LEFT			1		// direction of scroll - left
#define SCR_RIGHT			2		// ^ right
#define SCR_UP				3		// ^ up
#define SCR_DOWN			4		// ^ down.

//image file types
#define IMG_RAW				0
#define IMG_JPG				1
#define IMG_PNG				2
#define IMG_BMP				3
#define IMG_TGA				4
#define IMG_PGM				5		// 1bpp only (P5)
#define IMG_GIF				6		// first frame only, read only
#define IMG_PSD				7		// read only
#define IMG_PPM				8		// P6/P3
#define IMG_PBM				9		// PF	32bit float
#define IMG_ICO				10		// .ico and icon from .exe (Windows only)

#define IMG_KEEPALPHA		0x100	// use with png to retain alpha channel

// flip frame direction
#define FF_HORIZONTAL		0x01
#define FF_VERTICAL			0x02
#define FF_VERTANDHORIZ		0x04	// diagonally

// driver status
#define LDRV_CLOSED			0x00	// is not opened
#define LDRV_READY			0x01	// is open
#define LDRV_DISENGAGED		0x02	// display is opened but paused

#define LDRV_DISPLAY		1		// driver enumeration mode
#define LDRV_PORT			2

// Print rendering flags
#define PF_DONTRENDER		0x000001	// inhibit write to surface (used with calulating rects)
#define PF_CLIPDRAW			0x000002	// restricts glyph rendering to the bounding rect
#define PF_CLIPTEXTH		0x000004	// sets whether function will render part of character on or over rect area
#define PF_CLIPTEXTV		0x000008	// ie, render glyph only if fits within bounding region. PF_CLIPDRAW must be set.
#define PF_CLIPWRAP			0x000010	// wrap text. setting this flag will result in PF_CLIPDRAW being set.
#define PF_NOESCAPE			0x000020	// disable '\n' line escaping
#define PF_NODEFAULTCHAR	0x000040	// do not use font defined default glyph in place of a missing char/unsupported code point
#define PF_FIXEDWIDTH		0x000080	// use fixed width
#define PF_USELASTX			0x000100	// use x end point for next starting point
#define PF_USELASTY			0x000200	// use y end point for next starting point
#define PF_NEWLINE			PF_USELASTY
#define PF_USELASTXY		PF_USELASTX|PF_USELASTY
#define PF_RESETX			0x000400	// reset x start point to bx1 bounding point
#define PF_RESETY			0x000800	// reset y start point to by1 bounding point
#define PF_RESETXY			PF_RESETX|PF_RESETY
#define PF_LEFTJUSTIFY		0x001000	// text is left justified by default
#define PF_MIDDLEJUSTIFY	0x002000	// middle justify line,	requires PF_CLIPWRAP
#define PF_RIGHTJUSTIFY		0x004000	// right justify line, requires PF_CLIPWRAP
#define PF_DISABLEAUTOWIDTH 0x008000	// disables wide character (bdf) auto width calculation - is overridden by PF_FIXEDWIDTH
#define PF_GLYPHBOUNDINGBOX	0x010000	// enclose each glyph within a rectangle, is not overridden by 'PF_DONTRENDER'
#define PF_TEXTBOUNDINGBOX	0x020000	// draw a dotted border around printed area
#define PF_INVERTTEXTRECT	0x040000	// invert text rectangle
#define PF_GETTEXTBOUNDS	0x080000	// used internally with lGetTextMetricsList()
#define PF_DONTFORMATBUFFER 0x100000	// disable print formatting
#define PF_WORDWRAP			0x200000	// enable word wrap
#define PF_INVERTGLYPH1		0x400000
#define PF_INVERTGLYPH2		0x800000
#define PF_VERTICALLTR		0x1000000
#define PF_VERTICALRTL		0x2000000	// render top down from right to left
#define PF_HORIZONTALRTL	0x4000000	// render from right to left. Intended for East Asian (CJK) scripts.
#define PF_FORCEAUTOWIDTH	0x8000000

#define PF_INVERTTEXT		PF_INVERTTEXTRECT
#define PF_IGNOREFORMATTING	PF_DONTFORMATBUFFER


//		option					  index
#define lOPT_PCD8544_CS				0	// CS_1:CS_2:CS_3.  Set chip select line. See 'pcd8544_sio.h'
#define lOPT_PCD8544_HFLIP			1	// 0:1
#define lOPT_PCD8544_INVERT 		2	// 0:1
#define lOPT_PCD8544_STRUCT			3

#define lOPT_PCF8814_CS				0	// Unused: TODO: Set chip select lines, see 'pcd8814_spi.h' for details
#define lOPT_PCF8814_HFLIP			1	// 0:1
#define lOPT_PCF8814_INVERT			2	// 0:1
#define lOPT_PCF8814_STRUCT			3

#define lOPT_PCF8833_CONTRAST		1	// contrast range: 0-63

#define lOPT_S1D15G10_CONTRAST		1	// contrast range: coarse 0-63, fine 0-7. set via ((coarse<<8)|fine)

#define lOPT_S1D15G14_CONTRAST		1	// contrast range: 0-127

#define lOPT_USB13700EXP_STRUCT		0
#define lOPT_USB13700EXP_FRMLENGTH	1	// frameLength
#define lOPT_USB13700EXP_CLKPOL		2	// clock polarity
#define lOPT_USB13700EXP_CLKPHASE	3	// clock phase
#define lOPT_USB13700EXP_BITCLOCK	4	// bitclock
#define lOPT_USB13700EXP_PRESCALER	5	// prescaler. Bitrate is calculated using formula: 60000000/(prescaler*[bitclock+1])

#define lOPT_USBD480_STRUCT			0
#define lOPT_USBD480_BRIGHTNESS		1	// set backlight brightness level between 0 and 255
#define lOPT_USBD480_TOUCHPOSITION	2	// wait for and get a touch report. this method blocks. use lOPT_USBD480_TOUCHCB for non-blocking
#define lOPT_USBD480_TOUCHCB		3	// setup touch input callback. view examples/touch.c for details
#define lOPT_USBD480_TOUCHCBUSERPTR	4

#define lOPT_SBUI_STRUCT			0
#define lOPT_SBUI_GESTURECB			1
#define lOPT_SBUI_DKCB				2
#define lOPT_SBUI_UDATAPTR			3
#define lOPT_SBUI_GESTURECFG		4
#define lOPT_SBUI_SETDK				5
#define lOPT_SBUI_RECONNECT			6
#define lOPT_SBUI_RSTATS			7
#define lOPT_SBUI_RSTATSRESET		8

#define lOPT_SED1565_HFLIP			0	// 0:1

#define lOPT_LEDCARD_MODE			0	// FIXED:SCROLL 0:1

#define lOPT_KS0108_DELAY			0	// delay between writes to controller, use if experiencing corruption. default is 1
										// eg; drv->dd->setOption(drv, lOPT_KS0108_DELAY, 3)
#define lOPT_T6963C_DELAY			0	// as above

#define lOPT_SED1335_DELAY			0	// as above
#define lOPT_SED1335_XTAL			1	// sed1335 crystal speed. Adjusting XTAL speed will restart controller
#define lOPT_SED1335_TLAYER			2	// total layers. defaults to 1. Adjusting TLAYER's will cause a controller restart
#define lOPT_SED1335_ALAYER			3	// active layer. defaults to first layer, ie; layer '0'

#define lOPT_U13700D_STRUCT 		0	// get pointer to internal structure.

#define lOPT_G15_STRUCT				0	// get pointer to TMYLCDG15 struct. g15/g15display.h for details
#define lOPT_G15_SOFTKEYCB			1	// soft button call back, see wa.c or irc.c for an example of use
#define lOPT_G15_PRIORITY			2	// sync/async update mode
#define lOPT_G15_DEVICE				3	// device index

#define lOPT_G19_STRUCT				0	// get pointer to TMYLCDG19 struct. g19/g19display.h for details
#define lOPT_G19_SOFTKEYCB			1	// soft button call back, see wa.c or irc.c for an example of use
#define lOPT_G19_PRIORITY			2	// sync/async update mode
//#define lOPT_G19_DEVICE				3	// device index

#define lOPT_G15LU_STRUCT			0	// get pointer to TMYLCDG15 struct, see irc.c and g15/g15display.h for more details
#define lOPT_G15LU_SOFTKEYCB		1	// soft button call back, see wa.c or irc.c for an example of use
#define lOPT_G15LU_MLEDS			2
#define lOPT_G15LU_KBBRIGHTNESS	 	3
#define lOPT_G15LU_LCDBRIGHTNESS	4
#define lOPT_G15LU_LCDCONTRAST		5

#define lOPT_DDRAW_STRUCT			0	// get pointer to internal structure.
#define lOPT_DDRAW_HWNDTARGET		1	// handle to callback window
#define lOPT_DDRAW_GETHWND			2	// get internal window handle 
#define lOPT_DDRAW_SHOW				3
#define lOPT_DDRAW_HIDE				4

#define lOPT_SDL_STRUCT				0	// as above
#define lOPT_GL_STRUCT				0	// as above


#define lOPT_FT245_BAUDRATE			0	// adjust FT245 baudrate
#define lOPT_FT245_DIVISOR			1	// set devisor for non standard baudrates. Refer to FTDI's FT245BM datasheet for details
#define lOPT_FT245_BITBANG			2	// switches bit bang mode on or off (0/1)
#define lOPT_FT245_LATENCY			3	// set receive buffer latency timer from 2ms to 255ms in 1ms intervals
#define lOPT_FT245_IODIR			4	// set direction of the 8bit data bus under bit bang mode. See FT245BM	datasheet for details

#define lOPT_FTDI_STRUCT			0	// TODO:
#define lOPT_FTDI_DEVICE			1	// TODO: usb ftdi device index. 0=first device, 1=second and so on
#define lOPT_FTDI_BAUDRATE			2	// adjust chips baudrate
#define lOPT_FTDI_BITBANG			3	// switches bit bang mode on or off (0/1)
#define lOPT_FTDI_LATENCY			4	// set receive buffer latency timer from 2ms to 255ms in 1ms intervals
#define lOPT_FTDI_IODIR				5	// set direction of the 8bit data bus under bit bang mode. See FT245BM	datasheet for details

#define CAP_STATE_OFF				0
#define CAP_STATE_ON				1

enum _HWCAPS
{
	CAP_NONE = 0,			// unused
	CAP_BACKBUFFER,			// Use back buffer and optimised frame transfers if controller supports partial area updates
	CAP_HTML_NUM_REF,		// HTML character and entity references supported, ie; '&#36215;', '&amp;', etc..
	CAP_HTML_CHAR_REF,
	CAP_BMP,				// Bitmap supported
	CAP_PNG_READ,			// PNG read is enabled
	CAP_PNG_WRITE,			// PNG write is enabled
	CAP_JPG_READ,			// Jpeg read is enabled
	CAP_JPG_WRITE,			// Jpeg write is enabled
	CAP_GIF,				// read only, write will never be a implemented
	CAP_TGA,				// TGA supported
	CAP_BDF_FONT,			// BDF fonts supported
	CAP_BITMAP_FONT,		// Image bitmap font routines compiled in
	CAP_PGM,				// PGM images supported
	CAP_ICO,				// Loading icons from both .ico and .exe (Windows only)
	CAP_DRAW,				// Primitive API compiled in
	CAP_ROTATE,				// Rotate primitives
	CAP_FONTS,				// Build font engine
	CAP_CDECODE,			// Build character decoding
	CAP_PIXELEXPORTS,		// Pixel operation exports created. (ie; frm->pops->set() is available as lSetPixel())
	CAP_APISYNC,			// API Synchronization enabled
	CAP_PTHREADS,			// Using pthreads (pthreadGC2.dll) in place of MS Windows synchronization API
	CAP_PRINT,				// Print API enabled
	CAP_SCROLL,				// Scroll API enabled
	CAP_BIG5,				// Internal Big5 compiled in
	CAP_SJISX0213,			// Japanese supported (compiled in only)
	CAP_HZGB2312,			// HZ-GB2312 support built
	CAP_NULLDISPLAY,		// NULL display is compiled and selectable
	CAP_NULLPORT,			// NULL port driver is compiled
	CAP_DDRAW,				// DirectDraw virtual display is compiled
	CAP_SDL,				// SDL virtual display compiled
	CAP_OPENGL,				// OpenGL virtual display compiled
	CAP_PCD8544,			// PCD8544 from the Nokia mobile phone series (eg, 3210)
	CAP_PCF8814,			// PCF8514 from the Nokia mobile phone series (eg, 3510 mono)
	CAP_PCF8833,			// PCF8833 support
	CAP_S1D15G10,			// S1D15G10 support
	CAP_S1D15G14,			// Nokia 3510i (colour)
	CAP_SED1565,			// SED1565 controller supported
	CAP_SED1335,			// SED1330/13305/1335/1336 display driver
	CAP_T6963C,				// Toshiba T6963 controller supported
	CAP_KS0108,				// KS0108/HD61203 controller supported
	CAP_G15LIBUSB,			// Display on the Logitech G15 keyboard accessed through libusb.
	CAP_G15DISPLAY,			// Display on the Logitech G15 keyboard.
	CAP_G19DISPLAY,			// Display on the Logitech G19 keyboard.
	CAP_USBD480DLL,			// USBD480 available and accessed through USBD480_lib.dll
	CAP_USBD480LIBUSB,		// USBD480 access through libusb available
	CAP_USBD480LIBUSBHID,	// USBD480 access through libusb and HID (touch input) available
	CAP_USB13700DLL,		// USB13700 through display_lib_USB13700.dll
	CAP_USB13700LIBUSB,		// USB13700 through libusb13700
	CAP_USB13700EXP,		// USB13700 expansion port. port0 for control and port1 for SPI
	CAP_SBUI,				// Switchblade32.dll
	CAP_LEDCARD,			// A PIC driven, serially controlled 40x12 LED display
	CAP_WINIO,				// WinIO port driver with winio.dll compiled in, requires winio.sys
	CAP_OPENPARPORT,		// Open ParPort (Win2k only?) supported
	CAP_DLPORTIO,			// DLPortIo LINX driver supported, requires dlportio.dll/sys
	CAP_SERIAL,				// Serial port driver compiled in
	CAP_FT245,				// FTDI FT245BM USB driver. FTDIChip.com's (MS Windows) D2XX driver should be preinstalled before using
	CAP_FTDI,				// FT245BM and other FTDI chips through libFTDI
	CAP_MMX,				// MMX replacement routines for memcpy (and other stuff) compiled in.
	CAP_MMX2,				// includes CAP_MMX
	CAP_3DNOW,				// includes CAP_MMX and CAP_MMX2
	CAP_SSE,				
	CAP_DEBUG,				// Compiled in debug mode - is not runtime switchable
	CAP_DEBUG_FILEIO,		// ^^ display file open, close read and write requests
	CAP_DEBUG_MEM,			// General memory debugging and leak finding. refer to memory.h and mylcdmemory.h
	CAPS_TOTAL
};


// languages available
enum _CMT
{
	CMT_NONE = 0,				// don't perform character remapping, straight byte for byte relationship. ie, no conversion takes place
	CMT_UTF8 = 3001,			// UTF8
	CMT_UTF16,					// UTF16 is available only through the 'lxxxList()' API, or through lPrintxx() with PF_DONTFORMATBUFFER set. Uses BOM to determine endian.
	CMT_UTF16LE,				// as above, little endian. BOM is ignored.
	CMT_UTF16BE,				// as above, big endian. BOM is ignored.
	CMT_UTF32,					// UTF32 is available only through the 'lxxxList()' API (ucs4)
	CMT_BIG5,					// Traditional Chinese, Taiwan, Hong Kong SAR (CP950)
	CMT_GBK,					// Simplified Chinese, mainland China (CP936)
	CMT_GB18030,				// Chinese standard similar to GBK, mainland China
	CMT_HZ_GB2312,				// HZ GB.2312.1980
	CMT_EUC_CN,					// Extended Unix Code for Simplified Chinese using GB2312
	CMT_EUC_TW,					// Extended Unix Code for Traditional Chinese
	CMT_EUC_KR,					// Korean, EUC-KR (CP949)
	CMT_ISO2022_KR,				// Korean ISO-2022-KR (KSX1001)
	CMT_ISO2022_JP,				// Japanese ISO-2022-JP, single/double byte encoding
	CMT_JISX0213,				// Shift JIS, X 0213 planes 1 and 2
	CMT_JISX0208,				// EUC-JP (JIS-X-0208 code set 1)
	CMT_JISX0201,				// Japanese 8bit standard
	CMT_ISO_2022_JP_EUC_SJIS,	// Japanese encoding - auto detect between ISO-2022-JP, EUC-JP and Shift JIS
	CMT_TIS620,					// 8bit national Thai standard
	CMT_GB1988,					// 7bit Chinese variant of ASCII
	CMT_SYMBOL,
	CMT_DINGBATS,
	CMT_KOI8_R,
	CMT_KOI8_U,
	CMT_CP437,
	CMT_CP737,
	CMT_CP775,
	CMT_CP850,
	CMT_CP852,
	CMT_CP855,
	CMT_CP857,
	CMT_CP860,
	CMT_CP861,
	CMT_CP862,
	CMT_CP863,
	CMT_CP864,
	CMT_CP865,
	CMT_CP866,
	CMT_CP869,
	CMT_CP874,
	CMT_CP1250,
	CMT_CP1251,
	CMT_CP1252,
	CMT_CP1253,
	CMT_CP1254,
	CMT_CP1255,
	CMT_CP1257,
	CMT_CP1258,
	CMT_ISO8859_1,
	CMT_ISO8859_2,		// Latin alphabet no. 2 (Latin-2)
	CMT_ISO8859_3,		// South European (iso-ir-109, Latin-3)
	CMT_ISO8859_4,		// North European (iso-ir-110, Latin-3)
	CMT_ISO8859_5,		// Cyrillic languages (iso-ir-144)
	CMT_ISO8859_6,		// Arabic (RTL (bidi)text is unsupported within myLCD)
	CMT_ISO8859_7,		// Greek (iso-ir-126, ELOT 928)
	CMT_ISO8859_8,		// Hebrew (RTL (bidi) text unsupported within myLCD)
	CMT_ISO8859_9,		// Turkish (iso-ir-148, latin5)
	CMT_ISO8859_10,		// Nordic languages (iso-ir-157, latin6)
	CMT_ISO8859_11,		// Thai (tis620+nbsp)
	CMT_ISO8859_13,		// Baltic languages (Baltic rim, Latin-7)
	CMT_ISO8859_14,		// Celtic languages (iso-ir-199, Latin-8)
	CMT_ISO8859_15,		// (updated table similar to iso-8859-1, includes 'euro', Latin-9)
	CMT_ISO8859_16,		// South-Eastern European, French, Italian and Irish Gaelic (iso-ir-226, Latin-10)

	CMT_UNICODE	= CMT_UTF16,
	CMT_ASCII	= CMT_NONE,
	CMT_ISO8859 = CMT_ISO8859_1,
	CMT_GB2312	= CMT_HZ_GB2312,
	CMT_CP936	= CMT_GBK,
	CMT_CP950	= CMT_BIG5,
	CMT_EUC_JP	= CMT_JISX0208,
	CMT_SHIFTJIS= CMT_JISX0213,
	CMT_AUTO_JP = CMT_ISO_2022_JP_EUC_SJIS,

	CMT_DEFAULT = CMT_ISO8859_15
};


// BDF fonts
enum _FONTDEFSW
{
	lBaseFontW = 2001,
	LFTW_WENQUANYI9PT = lBaseFontW,
	LFTW_WENQUANYI12PT,
	LFTW_WENQUANYI36PT,
	LFTW_UNICODE,			// unicode.bdf
	LFTW_MONAU16,			// http://monafont.sourceforge.net/index-e.html
	LFTW_MONAU14,			// Shift_JIS is best displayed with Mona (MS GOTHIC)
	LFTW_MONA8X16A,
	LFTW_MONA7X14A,
	LFTW_18x18KO,
	LFTW_B34,
	LFTW_B28,
	LFTW_B26,
	LFTW_B24,
	LFTW_B16B,
	LFTW_B16,
	LFTW_B14,
	LFTW_B12,
	LFTW_B10,
	LFTW_4x6,
	LFTW_5x7,
	LFTW_5x8,
	LFTW_6x8_CP437,
	LFTW_6x9,
	LFTW_6x10,
	LFTW_6x12,
	LFTW_CLR6X12,
	LFTW_6x13,
	LFTW_6x13B,
	LFTW_6x13O,
	LFTW_7x13,
	LFTW_7x13B,
	LFTW_7x13O,
	LFTW_7x14,
	LFTW_7x14B,
	LFTW_8x13,
	LFTW_9x18,
	LFTW_9x18B,
	LFTW_10x20,
	LFTW_13x26,
	LFTW_NIMBUS14,
	LFTW_SNAP,
	LFTW_ROUGHT18,
	LFTW_HELVB18,
	LFTW_COMICSANS20,
	LFTW_HELVR08,
	LFTW_HELVR10,
	LFTW_HELVR12,
	LFTW_HELVR14,
	LFTW_HELVR18,
	LFTW_HELVR24,
	LFTW_COURR08,
	LFTW_COURR10,
	LFTW_COURR12,
	LFTW_COURR14,
	LFTW_COURR18,
	LFTW_COURR24,
	LFTW_HERIR08,
	LFTW_HERIR10,
	LFTW_HERIR12,
	LFTW_HERIR14,
	LFTW_HERIR18,
	LFTW_HERIR24,
	LFTW_LUTRS08,
	LFTW_LUTRS10,
	LFTW_LUTRS12,
	LFTW_LUTRS14,
	LFTW_LUTRS18,
	LFTW_LUTRS19,
	LFTW_LUTRS24,
	LFTW_ETL14,
	LFTW_ETL16,
	LFTW_ETL24,
	LFTW_CU12,
	LFTW_CUALT12,
	LFTW_XSYMB1_12,
	LFTW_XSYMB0_12,
	LFTW_NCENR10,
	LFTW_TIMR10,
	LFTW_UTRG__10,
	LFTW_PROOF9X16,
	LFTW_SCREEN8X16,
	LFTW_KOI5X8,
	LFTW_KOI7X14,
	LFTW_KOI9X18,
	LFTW_KOI10X20,
	LFTW_KOI12X24B,
	LFTW_WQYUNIBIT,
	LFTW_UNICODE36,
	LFTW_UNICODE72,
	LFTW_TRAJAN132,
	LFTW_LEVENIMMT132,
	LFTW_ACTIONMAN132,
	LFTW_RACER96,
	LFTW_RACER102,
	LFTW_RACER132,
	LFTW_198FIVE132,
	LFTW_198FIVE162,
	LFTW_76LONDON34,
	LFTW_76LONDON38,
	LFTW_76LONDON150,
	LFTW_NOTOSANS36,
	LFTW_NAMCO91,
	LFTW_END
};


// TGA font bitmaps
enum _FONTDEFSA
{
	lBaseFontA = 1001,
	LFT_SMALLFONTS7X7 = lBaseFontA,
	LFT_LONG6X13,
	LFT_COMICSANSMS7X8,
	LFT_COURIERNEWCE8,
	LFT_COMICSANSMS8X9,
	LFT_GUNGSUHCHE8X8,
	LFT_F3,
	LFT_GEORGIA9,
	LFT_SIMPSONS8,
	LFT_XFILES12,
	LFT_GOTHAMNIGHTS,
	LFT_BLACKADDERII,
	LFT_ARIAL,
	LFT_COURIER10,
	LFT_F0,
	LFT_COMICSANSMS15X13,
	LFT_NTR9,
	LFT_FONT0,
	LFT_ARIALBOLD,
	LFT_COURIER14,
	LFT_ARIALBLACK9X10,
	LFT_ARIALITALIC,
	LFT_BASICFONT,
	LFT_F2,
	LFT_FONT1,
	LFT_QUAKEA,
	LFT_QUAKEB,
	LFT_DOTUMCHE24X24,
	LFT_SIMPSONS14,
	LFT_LANSBURY,
	LFT_SFGRANDEZZA,
	LFT_JAPANESE_KANJI,
	LFT_DRAGONFLY,
	LFT_RODDY,
	LFT_MISSCLUDE,
	LFT_VIT,
	LFT_VITBOLD,
	LFT_BLOCKUP,
	LFT_LIQUIS,
	LFT_CHINATOWN,
	LFT_CHOWFUN,
	LFT_WONTON,
	LFT_SANTAMONL,
	LFT_SANTAMON,
	LFT_MARATHILEKHANI,
	LFT_KORV,
	LFT_XFILES20,
	LFT_CHARMING,
	LFT_GEORGIA24,
	LFT_GEORGIA24I,
	LFT_LUCKY,
	LFT_TRUMANIA,
	LFT_ANGLICANTEXT,
	LFT_COMICSANSMS48X48,
	LFT_GUNGSUHCHE24X36,
	LFT_ARAKAWA,
	LFT_TETRICIDE,
	LFT_COURIERNEWCE56,
	LFT_INTERDIMENSIONAL12,
	LFT_INTERDIMENSIONAL16,
	LFT_INTERDIMENSIONAL18,
	LFT_INTERDIMENSIONAL,
	LFT_PORSCHE911,
	LFT_F1,
	LFT_LDW20,
	LFT_LDW16,
	LFT_LDW10,

	LFT_MODEM_16,
	LFT_MODEM_18,
	LFT_MODEM_20,
	LFT_MODEM_24,
	LFT_MODEM_32,
	LFT_MODEM_32B,
	LFT_04B2_10,
	LFT_04B2_16,
	LFT_04B2_20,
	LFT_AKBAR_16,
	LFT_AKBAR_18,
	LFT_AKBAR_20,
	LFT_AKBAR_24,
	LFT_AKBAR_32,
	LFT_ARIAL_16,
	LFT_ARIAL_18,
	LFT_ARIAL_20,
	LFT_ARIAL_24,
	LFT_ARIAL_32,
	LFT_ASIANDINGS_20,
	LFT_ASIANDINGS_24,
	LFT_ASIANDINGS_32,
	LFT_BATANG_16,
	LFT_BATANG_18,
	LFT_BATANG_20,
	LFT_BATANG_24,
	LFT_BATANG_32,
	LFT_BATANGCHE_16,
	LFT_BATANGCHE_18,
	LFT_BATANGCHE_20,
	LFT_BATANGCHE_24,
	LFT_BATANGCHE_32,
	LFT_COMICSANS_16,
	LFT_COMICSANS_18,
	LFT_COMICSANS_20,
	LFT_COMICSANS_24,
	LFT_COMICSANS_32,
	LFT_COURIERNEW_16,
	LFT_COURIERNEW_16B,
	LFT_COURIERNEW_18,
	LFT_COURIERNEW_18B,
	LFT_COURIERNEW_20,
	LFT_COURIERNEW_20B,
	LFT_COURIERNEW_24,
	LFT_COURIERNEW_24B,
	LFT_COURIERNEW_32,
	LFT_COURIERNEW_32B,
	LFT_DOTUM_16,
	LFT_DOTUM_18,
	LFT_DOTUM_20,
	LFT_DOTUM_24,
	LFT_DOTUM_32,
	LFT_DOTUMCHE_16,
	LFT_DOTUMCHE_18,
	LFT_DOTUMCHE_20,
	LFT_DOTUMCHE_24,
	LFT_DOTUMCHE_32,
	LFT_GEORGIA_16,
	LFT_GEORGIA_18,
	LFT_GEORGIA_20,
	LFT_GEORGIA_24,
	LFT_GEORGIA_32,
	LFT_GULIM_16,
	LFT_GULIM_18,
	LFT_GULIM_20,
	LFT_GULIM_24,
	LFT_GULIM_32,
	LFT_GUNGSUH_16,
	LFT_GUNGSUH_18,
	LFT_GUNGSUH_20,
	LFT_GUNGSUH_24,
	LFT_GUNGSUH_32,
	LFT_GUNGSUHCHE_16,
	LFT_GUNGSUHCHE_18,
	LFT_GUNGSUHCHE_20,
	LFT_GUNGSUHCHE_24,
	LFT_GUNGSUHCHE_32,
	LFT_HANSHAND_16,
	LFT_HANSHAND_18,
	LFT_HANSHAND_20,
	LFT_HANSHAND_24,
	LFT_HANSHAND_32,
	LFT_MSGOTHIC_16,
	LFT_MSGOTHIC_18,
	LFT_MSGOTHIC_20,
	LFT_MSGOTHIC_24,
	LFT_MSGOTHIC_32,
	LFT_SHUI_16,
	LFT_SHUI_18,
	LFT_SHUI_20,
	LFT_SHUI_24,
	LFT_SHUI_32,
	LFT_ARIAL_10,
	LFT_ARIAL_10B,
	LFT_ARIAL_12,
	LFT_ARIAL_12B,
	LFT_ICONS_16,
	LFT_ICONS_22,
	LFT_ICONS_32,
	LFT_ICONS2_16,
	LFT_ICONS2_22,
	LFT_ICONS2_32,
	LFT_IMPACT_14,
	LFT_IMPACT_16,
	LFT_IMPACT_18,
	LFT_IMPACT_20,
	LFT_IMPACT_24,
	LFT_IMPACT_32,
	LFT_LINOTYPE_10,
	LFT_LINOTYPE_12,
	LFT_LINOTYPE_14,
	LFT_LINOTYPE_16,
	LFT_LINOTYPE_18,
	LFT_LINOTYPE_20,
	LFT_LINOTYPE_24,
	LFT_LINOTYPE_32,
	LFT_LINOTYPE_8,
	LFT_LUCIDACONSOLE_10,
	LFT_LUCIDACONSOLE_12,
	LFT_LUCIDACONSOLE_14,
	LFT_LUCIDACONSOLE_16,
	LFT_LUCIDACONSOLE_18,
	LFT_LUCIDACONSOLE_20,
	LFT_LUCIDACONSOLE_24,
	LFT_LUCIDACONSOLE_32,
	LFT_LUCIDASANS_10,
	LFT_LUCIDASANS_12,
	LFT_LUCIDASANS_14,
	LFT_LUCIDASANS_16,
	LFT_LUCIDASANS_18,
	LFT_LUCIDASANS_20,
	LFT_LUCIDASANS_24,
	LFT_LUCIDASANS_32,
	LFT_SCRIPT_24,
	LFT_SCRIPT_24B,
	LFT_SCRIPT_32,
	LFT_SMALLFONTS_5,
	LFT_SMALLFONTS_6,
	LFT_SMALLFONTS_7,
	LFT_SYMBOL_10,
	LFT_SYMBOL_12,
	LFT_SYMBOL_14,
	LFT_SYMBOL_16,
	LFT_SYMBOL_18,
	LFT_SYMBOL_20,
	LFT_SYMBOL_24,
	LFT_SYMBOL_32,
	LFT_SYSTEM_16,
	LFT_SYSTEM_8,
	LFT_TAHOMA_10,
	LFT_TAHOMA_12,
	LFT_TAHOMA_14,
	LFT_TAHOMA_16,
	LFT_TAHOMA_18,
	LFT_TAHOMA_20,
	LFT_TAHOMA_24,
	LFT_TAHOMA_32,
	LFT_TAHOMA_8,
	LFT_TERMINAL_12,
	LFT_TERMINAL_14,
	LFT_TERMINAL_18,
	LFT_TERMINAL_24,
	LFT_TERMINAL_32,
	LFT_TERMINAL_8,
	LFT_TERMINAL_9,
	LFT_TIMESROMAN_10,
	LFT_TIMESROMAN_11,
	LFT_TIMESROMAN_12,
	LFT_TIMESROMAN_14,
	LFT_TIMESROMAN_16,
	LFT_TIMESROMAN_18,
	LFT_TIMESROMAN_20,
	LFT_TIMESROMAN_24,
	LFT_TIMESROMAN_32,
	LFT_TIMESROMAN_8,
	LFT_TIMESROMAN_9,
	LFT_TREBUCHET_10,
	LFT_TREBUCHET_12,
	LFT_TREBUCHET_14,
	LFT_TREBUCHET_16,
	LFT_TREBUCHET_18,
	LFT_TREBUCHET_20,
	LFT_TREBUCHET_24,
	LFT_TREBUCHET_32,
	LFT_TREBUCHET_8,
	LFT_FIXEDSYS_8,
	LFT_ROMAN_32,

	LFT_END
};


#ifdef __cplusplus
extern "C" {
#endif

#include "mylcdtypes.h"

// returns pointer to version string
MYLCD_EXPORT char *
lVersion();

// Initialise library. returns a handle to the library.
// Pass font and map file path if not default location, as defined by lFontPath and lCharacterMapPath.
// eg; lOpen(L"path/to/font/directories/", L"path/to/mapfiles");
MYLCD_EXPORT THWD *lOpen (const wchar_t *fontPath, const wchar_t *mapPath);

// Closes all devices associated with hw and free resources
// close devices and free resources
MYLCD_EXPORT void lClose (THWD *hw);

// Activate and initialize display and/or device
// displayName is the device to activate. eg: "PCD8544".
// portName = name of port driver connecting above driver to hardware. eg: "WinIO", or "NULL" if none.
// width/height = actual pixel width/height of display
// returns display ID on success, 0 on failure
// 'displayName' and 'portName' are case sensitive.
// handle to display rect should be maintained (be valid) throughout session
MYLCD_EXPORT lDISPLAY
lSelectDevice (THWD *hw, const char *displayName, const char *portName, int width, int height, int lfrm_bpp_nn, int data, TRECT *rect);

// Close a previously opened device
// It is not necessary to call lCloseDevice() as device(s) are automatically shutdown at lClose()
MYLCD_EXPORT int
lCloseDevice (THWD *hw, const lDISPLAY did);

// Set a display dependant option (lOPT_xxx_xxxx)
// ('value' can not be null)
// returns 0 on failure, 1 on success
MYLCD_EXPORT int
lSetDisplayOption (THWD *hw, lDISPLAY did, int opt_option, intptr_t *value);

// Get a display dependant option (lOPT_xxxx)
// returns 0 on failure, 1 on success
MYLCD_EXPORT int
lGetDisplayOption (THWD *hw, lDISPLAY did, int opt_option, intptr_t *value);

// returns handle to a previously activated display
// returns NULL on failure
// typical usage: may use 'drv->pd->writeX' and '->read' to directly access comm port (where supported by driver)
MYLCD_EXPORT TDRIVER *
lDisplayIDToDriver (THWD *hw, lDISPLAY did);

// Suspend display updates
MYLCD_EXPORT int
lPauseDisplay (THWD *hw, lDISPLAY did);

// Resume a suspended display.
MYLCD_EXPORT int
lResumeDisplay (THWD *hw, lDISPLAY did);

// List compiled in drivers. See examples/enumdrv.c
MYLCD_EXPORT TREGDRV *
lEnumerateDriversBegin (THWD *hw, int ledrv_type);
MYLCD_EXPORT int
lEnumerateDriverNext (TREGDRV *drv);
MYLCD_EXPORT void
lEnumerateDriverEnd (TREGDRV *drv);

// Return reference ID of an activated driver
// refer to examples/usbd480BL.c for usage example
MYLCD_EXPORT lDISPLAY
lDriverNameToID (THWD *hw, const char *name, int ldrv_type);

// Create a new surface (frame) attached to root device tree 'hw'
// returns handle to new surface or NULL on error
// Release surface by calling lDeleteFrame()
MYLCD_EXPORT TFRAME *
lNewFrame (THWD *hw, int width, int height, ubyte lfrm_bpp_nn);

// Release frame
MYLCD_EXPORT int
lDeleteFrame (TFRAME *frame);

MYLCD_EXPORT void
lDeleteFrames (THWD *hw, const int groupId);

// Resize frame
// Set keepdata to 1 to retain as far as possible the original contents.
// returns 0 on error, positive on success
MYLCD_EXPORT int
lResizeFrame (TFRAME *frame, int newWidth, int newHeight, int keepdata);

// Create a duplicate of a frame including pixel buffer
// Release frame with lDeleteFrame()
MYLCD_EXPORT TFRAME *
lCloneFrame (TFRAME *src);

// Clear display screen
// Also clears the back buffer.
MYLCD_EXPORT int
lClearDisplay (THWD *hw);

// Clear frame
MYLCD_EXPORT int
lClearFrame (TFRAME *frame);

#define lClearFrameArea(a,b,c,d,e) lDrawRectangleFilled((a),(b),(c),(d),(e), ((TFRAME*)(a))->hw->render->backGround)

#if 1
// Load image on to frame
// Frame is resized to fit image
MYLCD_EXPORT int
lLoadImage (TFRAME *frame, const wchar_t *filename);


// flags
#define LOAD_RESIZE				0x01	// resize frame to match image
#define LOAD_PIXEL_CPY			0x02	// where available, direct pixel copy (faster) in place of setPixel. currently in use only with PNG
#define LOAD_SIZE_RESTRICT		0x04	// x/y  is the maximum width/height of image

MYLCD_EXPORT int
lLoadImageEx (TFRAME *frame, const wchar_t *filename, const int flags, const int x, const int y);

MYLCD_EXPORT TFRAME *
lNewImage (THWD *hw, const wchar_t *filename, const int lfrm_bpp_nn);
#endif


// Write frame to file
MYLCD_EXPORT int
lSaveImage (TFRAME *frame, const wchar_t *filename, const int img_type, int width, int height);


// find demensions of an image
// img_type, type of image expected, or -1 to guess image type
MYLCD_EXPORT int
lImageGetMetrics (const wchar_t *filename, int img_type, int *width, int *height, int *bpp);


// Returns a surface with specified text rendered
// use lDeleteFrame() to destroy
MYLCD_EXPORT TFRAME *
lNewString (THWD *hw, const int lfrm_bpp_nn, const int pf_flags, const int font, const char *formatstring, ...)
#ifndef __WIN64__
__attribute__((format(printf, 5, 6)))
#endif
;

MYLCD_EXPORT TFRAME *
lNewStringEx (THWD *hw, const TMETRICS *metrics, const int lfrm_bpp_nn, const int pf_flags, const int font, const char *formatstring, ...)
#ifndef __WIN64__
__attribute__((format(printf, 6, 7)))
#endif
;

MYLCD_EXPORT TFRAME *
lNewStringList (THWD *hw, const int lfrm_bpp, const int pf_flags, const int font, const UTF32 *glist, const int gtotal);

MYLCD_EXPORT TFRAME *
lNewStringListEx (THWD *hw, const TMETRICS *metrics, const int lfrm_bpp, const int pf_flags, const int font, const UTF32 *glist, const int gtotal);


// basic text rendering api
// string	null terminated buffer
// x,y		location in frame to print
// font		fontid
// style	draw method
MYLCD_EXPORT int
lPrint (TFRAME *frame, const char *string, int x, int y, int font, int lprt_style);

// as above but with formatted printing
MYLCD_EXPORT int
lPrintf (TFRAME *frame, int x, int y, int font, int lprt_style, const char *format, ...)
#ifndef __WIN64__
	__attribute__((format(printf, 6, 7)))
#endif
;

// extended rendering api with formatted text
// rect			rectangle defining rendering bounding box
// flags		PF_ render flags
// style		LPRT_ draw method flags
MYLCD_EXPORT int
lPrintEx (TFRAME *frame, TLPRINTR *rect, int font, int pf_flags, int lprt_style, const char *formatstring, ...)
#ifndef __WIN64__
	__attribute__((format(printf, 6, 7)))
#endif
;

// Set a glyph rendering filter
// Applies to BDF (LFTW_nnn) rendering only
// Check example trenderfilter.c for more details
// returns 1 on success, 0 on failure
MYLCD_EXPORT int
lSetRenderEffect (THWD *hw, int ltr_mode);

// free resources and any attached settings with regard to ltr_mode associated with 'font'
// (auto free'd at exit and with font flush/delete)
MYLCD_EXPORT void 
lRenderEffectReset (THWD *hw, const int font, const int ltr_mode);


// scroll frame 'src' through 'des'
// see examples 'clock.c', 'scroll.c' for more details
// returns handle scroll
// free with lDeleteScroll()
MYLCD_EXPORT TLSCROLLEX *
lNewScroll (TFRAME *src, TFRAME *des);

// scroll source by one unit (pixel)
MYLCD_EXPORT int
lUpdateScroll (TLSCROLLEX *s);

// Draw scroll on distination, does not update position
MYLCD_EXPORT int
lDrawScroll (TLSCROLLEX *s);

// Release scroll resources
MYLCD_EXPORT void
lDeleteScroll (TLSCROLLEX *s);

// Send frame to display(s)
// If back buffering is enabled and supported then delta updates are performed.
// lRefresh() blocks until operation is complete
MYLCD_EXPORT int
lRefresh (TFRAME *frame);

// Send buffer directly to display(s) backbuffer. No pixel conversions or area mapping is applied.
MYLCD_EXPORT int
lUpdate (const THWD *hw, const void *buffer, const size_t bufferLen);

// Send a frame area to display
// Not supported by every controller. ie; USBD480 and G15 will redirect to lRefresh()
MYLCD_EXPORT int
lRefreshArea (TFRAME *frame, const int x1, const int y1, const int x2, const int y2);

// As with lRefresh() but non-blocking as operation is handled though a dedicated thread.
// mode: 0:buffer is copied, 1:buffers are swapped.
MYLCD_EXPORT int
lRefreshAsync (TFRAME *frame, const int mode);


// Get the colour mask of frame.
// eg; for RGB0888 frames, LMASK_RED|LMASK_BLUE would return 0x00FF00FF
MYLCD_EXPORT int
lGetRGBMask (const TFRAME *frame, const int lmask_colour);


// Erase frame contents
MYLCD_EXPORT int
lClearFrameClr (TFRAME *frame, const int colour);


// Find the smallest rectangle that will fit the contents of frame
MYLCD_EXPORT int
lGetImageBoundingBox (TFRAME *frame, TLPOINTEX *p);


// Copy src surface to des to location x,y
MYLCD_EXPORT int
lDrawImage (TFRAME *src, TFRAME *des, const int x, const int y);


// draws 'src' image with 'mask' on to 'des'
// mode: LMASK_operation flags
// see example 'mask1/2.c' for more details
MYLCD_EXPORT int
lDrawMask (TFRAME *src, TFRAME *mask, TFRAME *des, int maskXOffset, int maskYOffset, int lmask_mode);

// Draw masked src on to des with alpha applied to src
MYLCD_EXPORT int
lDrawMaskA (const TFRAME *src, const TFRAME *mask, TFRAME *des, const int desXoffset, const int desYoffset, const float alpha);

MYLCD_EXPORT int
lDrawMaskAEx (const TFRAME *src, const TFRAME *mask, TFRAME *des, const int Xoffset, const int Yoffset,\
			  const int srcX1, const int srcY1, const int srcX2, const int srcY2, const float alpha);

// miscellaneous primitives
MYLCD_EXPORT int
lDrawPolyLine (TFRAME *frame, T2POINT *lppt, int tLines, const int colour);

MYLCD_EXPORT int
lDrawPolyLineEx (TFRAME *frame, TLPOINTEX *pt, int tLines, const int colour);

MYLCD_EXPORT int
lDrawPolyLineTo (TFRAME *frame, TPOINTXY *lppt, int tnodes, const int colour);

MYLCD_EXPORT int
lDrawPolyLineDottedTo (TFRAME *frame, T2POINT *lppt, int tnodes, const int colour);

MYLCD_EXPORT int
lDrawCircle (TFRAME *frame, int xcenter, int ycenter, int radius, const int colour);

MYLCD_EXPORT int
lDrawCircleFilled (TFRAME *frame, int xcenter, int ycenter,int radius, const int colour);

MYLCD_EXPORT int
lDrawEllipse (TFRAME *frame, int x, int y, int r1, int r2, const int colour);

MYLCD_EXPORT int
lDrawArc (TFRAME *frame, int x, int y, int r1, int r2, double a1, double a2, const int colour);

MYLCD_EXPORT int
lDrawEnclosedArc (TFRAME *frame, int x, int y, int r1, int r2, double a1, double a2, const int colour);

MYLCD_EXPORT int
lDrawLine (TFRAME *frame, int x1, int y1, int x2, int y2, const int colour);

MYLCD_EXPORT int
lDrawLineDotted (TFRAME *frame, int x1, int y1, int x2, int y2, const int colour);

MYLCD_EXPORT int
lDrawRectangle (TFRAME *frame, int x1, int y1, int width, int height, const int colour);

MYLCD_EXPORT int
lDrawRectangleFilled (TFRAME *frame, int x1, int y1, int width, int height, const int colour);

MYLCD_EXPORT int
lDrawRectangleDotted (TFRAME *frame, int x1, int y1, int width, int height, const int colour);

MYLCD_EXPORT int
lDrawRectangleDottedFilled (TFRAME *frame, int x1, int y1, int width, int height, const int colour);

MYLCD_EXPORT int
lDrawTriangle (TFRAME *frame, int x1, int y1, int x2, int y2, int x3, int y3, const int colour);

MYLCD_EXPORT int
lDrawTriangleFilled (TFRAME *frame, int x1, int y1, int x2, int y2, int x3, int y3, const int colour);

MYLCD_EXPORT int
lFloodFill (TFRAME *frame, int x, int y, const int fillColour);

MYLCD_EXPORT int
lEdgeFill (TFRAME *frame, int x, int y, int fillColour, const int edgeColour);

MYLCD_EXPORT int
lBlur (TFRAME *src, TFRAME *workingBuffer, const int applyAlpha, const int radius);

MYLCD_EXPORT int
lBlurArea (TFRAME *frame, const int x1, const int y1, const int x2, const int y2, const int radius_iter);

MYLCD_EXPORT int
lBlurImage (TFRAME *frame, const int blurOp, const int radius_iter);

MYLCD_EXPORT int
lConvolve2D (TFRAME *in, TFRAME *out, float *const kernel, const int kernelSizeX, const int kernelSizeY);

MYLCD_EXPORT int
lConvolve2DSeparable (TFRAME *in, TFRAME *out, const float* const kernelX, const int kSizeX, const float* const kernelY, const int kSizeY);


// moves an area of frame within frame by 'n' pixels
// lmov_mode sets what should happen to the area thats just been uncovered by the move
// ie, if one was moving left this would be the right hand side the move
// options are to loop from, dump, set all or clear pixels.
// see example 'anitest.c' for more details
MYLCD_EXPORT int
lMoveArea (TFRAME *frame, int x1, int y1, int x2, int y2, int n_pixels, int lmov_mode, int lmov_direction);

// basic frame copying
MYLCD_EXPORT int
lCopyFrame (TFRAME *from, TFRAME *to);

MYLCD_EXPORT int
lCopyArea (TFRAME *from, TFRAME *to, int dx, int dy, int x1, int y1, int x2, int y2);

MYLCD_EXPORT int
lCopyAreaA (TFRAME *from, TFRAME *to, int dx, int dy, int x1, int y1, int x2, int y2, float alpha);

MYLCD_EXPORT int
lCopyAreaEx (TFRAME *from, TFRAME *to, int dx, int dy, int x1, int y1, int x2, int y2, int scaleh, int scalev, int lcass_mode);

MYLCD_EXPORT int
lCopyAreaScaled (TFRAME *from, TFRAME *to, int src_x, int src_y, int src_width, int src_height, int dest_x, int dest_y, int dest_width, int dest_height, int lcass_mode);

// invert frame
MYLCD_EXPORT int
lInvertFrame (TFRAME *frame);

// invert an area of frame
MYLCD_EXPORT int
lInvertArea (TFRAME *frame, int x1, int y1, int width, int height);

// flip frame vertically, horizontally or both.
// destination frame _must_ be the the same size or larger than source
MYLCD_EXPORT int
lFlipFrame (TFRAME *src, TFRAME *des, int ff_flag);

// basic rotation api
// view example 'anitest' for more details
MYLCD_EXPORT int
lRotate (TFRAME *src, TFRAME *des, int desx, int desy, double angle);

// rotates 'src' frame on to 'des' on a 3D plane
// see example 'rotatetest.c' for more details
MYLCD_EXPORT int
lRotateFrameEx (TFRAME *src, TFRAME *des, const float xang, const float yang, const float zang, float flength, const float zoom, const int destx, const int desty);

MYLCD_EXPORT int
lRotateFrameR90 (TFRAME *frame);

MYLCD_EXPORT int
lRotateFrameL90 (TFRAME *frame);

// calculates rotation of individual pixels by angle degrees
MYLCD_EXPORT void
lRotateX (const float angle, const float y, const float z, float *yr, float *zr);

MYLCD_EXPORT void
lRotateY (const float angle, const float x, const float z, float *xr, float *zr);

MYLCD_EXPORT void
lRotateZ (const float angle, const float x, const float y, float *xr, float *yr);

MYLCD_EXPORT void
lPoint3DTo2D (float x, float y, float z, float flength, float camz, int x0, int y0, int *ptx, int *pty);

// TGA font enumeration api
// (single bitmap image fonts, library can support PNG or BMP bitmaps if requested)
// see example 'tgatest.c' for more details
MYLCD_EXPORT TENUMFONT *
lEnumFontsBeginA (THWD *hw);

MYLCD_EXPORT int
lEnumFontsNextA (TENUMFONT *enf);

MYLCD_EXPORT void
lEnumFontsDeleteA (TENUMFONT *enf);

// BDF font enumeration api
// see example 'bdftest.c' for more details
MYLCD_EXPORT TENUMFONT *
lEnumFontsBeginW (THWD *hw);

MYLCD_EXPORT int
lEnumFontsNextW (TENUMFONT *enf);

MYLCD_EXPORT void
lEnumFontsDeleteW (TENUMFONT *enf);

// get internal font handle
// returns a handle to either a TFONT or TWFONT struct
// (getfont())
MYLCD_EXPORT void *
lFontIDToFont (THWD *hw, int fontid);

// Register a bitmap font (TGA, PGM, BMP or PNG).
// font sohuld have no more than 256 chars
// see source file 'fonts.c' for more details
MYLCD_EXPORT int
lRegisterFontA (THWD *hw, const wchar_t *filename, TFONT *font);

// release font and free its resources
// if font is built then it will be unavailable for use
MYLCD_EXPORT int
lUnregisterFontA (THWD *hw, int fontid);

// Register a BDF font
// see source file 'fonts.c' for more details
MYLCD_EXPORT int
lRegisterFontW (THWD *hw, const wchar_t *filename, TWFONT *font);

// release font and free its resources
// if font or glyphs from font are built then these wil be
// unavailable for use after this call
MYLCD_EXPORT int
lUnregisterFontW (THWD *hw, int fontid);

// delete font data and glyphs from memory
// see examples 'tgatest.c' and 'bdftest.c' for more details
// font remains registered
MYLCD_EXPORT int
lFlushFont (THWD *hw, int fontid);

// load glyphs from one font in to another
// glyphs from fontid_prim take priority
// view clock.c for an example of use
MYLCD_EXPORT int
lMergeFontW (THWD *hw, int fontid_prim, int fontid_sec);

// unregister fonts releasing all resources
MYLCD_EXPORT void
lDeleteFonts (THWD *hw);

// set current language encoding
MYLCD_EXPORT int
lSetCharacterEncoding (THWD *hw, int cmt_id);
// get current language encoding
MYLCD_EXPORT int
lGetCharacterEncoding ();

// cycle through supported encodings, returning encoding alias, id (CMT_nnn) and filepath (if applicable)
// refer to examples/enumlang.c for more details
MYLCD_EXPORT int
lEnumLanguageTables (THWD *hw, char **alias, int *cmt_id, wchar_t **filepath);

// look up encoding id from alias
// aliases are assigned in cmap.c
// eg; "JISX0201" will return CMT_JISX0201
MYLCD_EXPORT int
lEncodingAliasToID (THWD *hw, const char *alias);

// return handle to glyph pointed to by either 'str' or 'wc'
// do not free or returned handle or else bad things will happen
MYLCD_EXPORT TWCHAR *
lGetGlyph (THWD *hw, const char *str, UTF32 wc, int fontid);

// get char block sise
MYLCD_EXPORT int
lGetCharMetrics (THWD *hw, const char *str, int fontid, int *width, int *height);

// returns dimensions of text if rendered
MYLCD_EXPORT int
lGetTextMetrics (THWD *hw, const char *str, int pf_flags, int fontid, int *width, int *height);

// will also build font
MYLCD_EXPORT int
lGetFontMetrics (THWD *hw, int fontid, int *width, int *height, int *ascent, int *descent, int *tchars);

// return number of code points within string, is similar to strlen() but will process string according to current mapping
MYLCD_EXPORT int
lCountCharacters (THWD *hw, const char *str);

// process string according to current mapping, generating a list of code points
// extract code points from string
// glist buffer size should be 'sizeof(UTF32)*total'
// returns number of characters added to list
MYLCD_EXPORT int
lCreateCharacterList (THWD *hw, const char *str, UTF32 *glist, int total);

// removes precached characters and those previously found to be missing (from font) from glist
// sorts code points in to descending order
// returns number of individual uncached characters remaining
MYLCD_EXPORT int
lStripCharacterList (THWD *hw, UTF32 *glist, int gtotal, TWFONT *font);

// preload characters from unicode code point 'from' to 'to'
// returns number of glyphs loaded
MYLCD_EXPORT int
lCacheCharacterRange (THWD *hw, UTF32 from, UTF32 to, int fontid);

// cache chars from buffer
// performs operation on supplied buffer (glist) which is stripped of invalid chars and sorted
// function will not reload a glyph thats already been cached
// returns number of glyphs loaded
MYLCD_EXPORT int
lCacheCharacterList (THWD *hw, UTF32 *glist, int gtotal, int fontid);

// load glyphs from glist in to memory
// performs operation on a copy of input buffer, glist is unmodified.
// returns number of glyphs loaded
MYLCD_EXPORT int
lCacheCharacterBuffer (THWD *hw, const UTF32 *glist, int gtotal, int fontid);

// Decode and load characters from string buffer, must be null terminated
// returns number of glyph's built
MYLCD_EXPORT int
lCacheCharacters (THWD *hw, const char *str, int fontid);

// Load font completely in to memory
// returns number of glyph's built
MYLCD_EXPORT int
lCacheCharactersAll (THWD *hw, int fontid);

// returns all code points currently in memory
// glist array should should be large enough for code points (gtotal*sizeof(UTF32) bytes)
// on success returns total code points
// returns -total if total cached glyphs is greater than gtotal
// see bdfdump.c for an example of use
MYLCD_EXPORT int
lGetCachedGlyphs (THWD *hw, UTF32 *glist, int gtotal, int fontid);

// as with 'lGetTextMetrics()' but performs operation on an already decoded and cached buffer.
// glyphs must be pre-loaded before calling.
// returns index to last computed char
// see bdfdump.c for an example of use
MYLCD_EXPORT int
lGetTextMetricsList (THWD *hw, const UTF32 *glist, int first, int last, int pf_flags, int fontid, TLPRINTR *rect);

// renders text
// returns index to last char rendered
// see bdfdump.c for an example of use
MYLCD_EXPORT int
lPrintList (TFRAME *frame, const UTF32 *glist, int firstInList, int totalToPrint, TLPRINTR *rect, int font, int flags, int lprt_style);

// convert code point or char reference within string buffer to unicode code point
// returns number of chars read from 'str'
// note: CMT_AUTO_JP/CMT_ISO_2022_JP_EUC_SJIS is not supported, instead use 'lDecodeCharacterBuffer()'
MYLCD_EXPORT int
lDecodeCharacterCode (THWD *hw, const char *str, UTF32 *chrout);

// as above, decodes a maximum of gtotal code points from buffer in to glist
// returns number of bytes read from buffer
MYLCD_EXPORT int
lDecodeCharacterBuffer (THWD *hw, const char *buffer, UTF32 *glist, int gtotal);

// sets space between adjacent horizontal glyphs, in pixels
// returns current value
MYLCD_EXPORT int
lSetFontCharacterSpacing (THWD *hw, int fontid, int pixels);

MYLCD_EXPORT int
lGetFontCharacterSpacing (THWD *hw, int fontid);

// set number of vertical pixels to skip between line wraps.
// returns current value
MYLCD_EXPORT int
lSetFontLineSpacing (THWD *hw, int fontid, int pixels);

// returns number of rows in pixels added when wrapping text.
MYLCD_EXPORT int
lGetFontLineSpacing (THWD *hw, int fontid);

// set the rendered width of glyphs without width
MYLCD_EXPORT int
lSetFontZeroWidthSpacing (THWD *hw, int fontid, int pixels);


// enable/disable HTML character and entity references, ie; &#36215;, &amp;, etc..
MYLCD_EXPORT void
lHTMLCharRefEnable();
MYLCD_EXPORT void
lHTMLCharRefDisable();

// enable/disable character combining
MYLCD_EXPORT void
lCombinedCharDisable ();
MYLCD_EXPORT void
lCombinedCharEnable ();

// block for n milliseconds
MYLCD_EXPORT void
lSleep (int n);

// Set library specific behaviour.
MYLCD_EXPORT int
lSetCapabilities (THWD *hw, unsigned int cap_flag, int value);

// Returns 1 if cap_flag is compiled in. eg; CAP_BIG5 is by default not compiled in.
// Otherwise returns 0
// View mylcdconfig.h for details of compile time options
MYLCD_EXPORT int
lGetCapabilities (THWD *hw, unsigned int flag);

// Utility function to aid converting from 1bpp to other formats.
MYLCD_EXPORT unsigned int
lFrame1BPPToRGB (TFRAME *frame, void *des, int des_lfrm_bpp_nn, const int clrLow, const int clrHigh);

// convert and copy from one BPP format to another in to a user buffer
// does not support 1bpp source format, use lFrame1BPPToRGB() instead.
MYLCD_EXPORT pConverterFn
lGetConverter (THWD *hw, int src_lfrm_bpp_nn, int des_lfrm_bpp_nn);
/* eg;
pConverterFn fn = lGetConverter(LFRM_BPP_32, LFRM_BPP_16);
if (fn){
	unsigned int *buffer = malloc(surface->height*surface->width*2);
	fn(surface, buffer);	// do convert and copy
	....
}
*/

// Set/Get how pixel write operations are performed per surface.
// Default is LSP_SET.
// Affects: Primitive, Print, Copy, Move and Clear APIs.
// Has no effect with 1bpp frames.
MYLCD_EXPORT void
lSetPixelWriteMode (TFRAME *frame, ubyte lsp_xxx);
MYLCD_EXPORT int
lGetPixelWriteMode (TFRAME *frame);

// Set foreground colour (ink) of print.
// 1bpp frames (LFRM_BPP_1) should  pass LSP_xxx in place of colour component.
MYLCD_EXPORT int
lSetForegroundColour (THWD *hw, int colour);
MYLCD_EXPORT int 
lGetForegroundColour (THWD *hw);

// Set/Get background colour (paper) of print.
// 1bpp frames (LFRM_BPP_1) should pass LSP_xxx in place of colour component.
MYLCD_EXPORT int
lSetBackgroundColour (THWD *hw, int colour);
MYLCD_EXPORT int
lGetBackgroundColour (THWD *hw);

//Set text filter attribute
MYLCD_EXPORT int
lSetFilterAttribute (THWD *hw, const int ltr_effect, const int attribute, const int value);
MYLCD_EXPORT int
lGetFilterAttribute (THWD *hw, const int ltr_effect, const int attribute);


#if __BUILD_PIXELPRIMITIVEEXPORTS__

// draw a pixel
MYLCD_EXPORT void
lSetPixel (const TFRAME *frame, const int x, const int y, const int colour);

// return pixel colour
MYLCD_EXPORT int
lGetPixel (const TFRAME *frame, const int x, const int y);

// get pixel without checking x,y are within the frame boundary
MYLCD_EXPORT int
lGetPixel_NB (const TFRAME *frame, const int x, const int y);

// set pixel without checking x,y are within the frame boundary
MYLCD_EXPORT void
lSetPixel_NB (const TFRAME *frame, const int x, const int y, const int value);

MYLCD_EXPORT void
lSetPixel_NBr (const TFRAME *frame, const int x, const int row, const int value);

MYLCD_EXPORT int
lGetPixel_NBr (const TFRAME *frame, const int x, const int row);

MYLCD_EXPORT void
lSetPixelf (const TFRAME *frame, const int x, const int y, const float r, const float g, const float b);

MYLCD_EXPORT int
lGetPixelf (const TFRAME *frame, const int x, const int y, float *r, float *g, float *b);

MYLCD_EXPORT void *
lGetPixelAddress (const TFRAME *frame, const int x, const int y);

#else

#define lSetPixel(f, x, y, c)			(f->pops->set(f, x, y, c))
#define lSetPixel_NB(f, x, y, c)		(f->pops->set_NB(f, x, y, c))
#define lSetPixel_NBr(f, x, row, c)		(f->pops->set_NBr(f, x, row, c))
#define lSetPixelf(f, x, y, r, g, b)	(f->pops->setf(f, x, y, r, g, b))
#define lGetPixel(f, x, y)				(f->pops->get(f, x, y))
#define lGetPixel_NB(f, x, y)			(f->pops->get_NB(f, x, y))
#define lGetPixel_NBr(f, x, row)		(f->pops->get_NBr(f, x, row))
#define lGetPixelf(f, x, y, r, g, b)	(f->pops->getf(f, x, y, r, g, b))
#define lGetPixelAddress(f, x, y)		(f->pops->getAddr(f, x, y))

#endif

#ifdef __cplusplus
}
#endif

#endif

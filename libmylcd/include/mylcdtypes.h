
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

#ifndef _MYLCDTYPES_H_
#define _MYLCDTYPES_H_


typedef uint8_t ubyte;
typedef uint8_t UTF8;	// 8 bits
typedef uint16_t UTF16;	// at least 16 bits
typedef uint32_t UTF32;	// at least 32 bits
typedef unsigned int lDISPLAY;
typedef struct TPORTDRIVER TPORTDRIVER;
typedef struct TDISPLAYDRIVER TDISPLAYDRIVER;
typedef struct TFRAME TFRAME;
typedef struct TFONT TFONT;
typedef struct TWFONT TWFONT;

typedef void (*pConverterFn) (TFRAME *frm, unsigned int *buffer);

typedef struct{
	int left;
	int right;
	int top;
	int btm;
}TRECT;

typedef struct{
	TDISPLAYDRIVER	*dd;
	TPORTDRIVER		*pd;
	TRECT	*rect;	/*	Area within a source frame which this display has claim to.
						This handle must be created and maintained throughout runtime by client.*/
	unsigned int *ctrl;
}TDRIVER;

typedef struct{
	int x;
	int y;
	int width;		// max width, or if minus then increase width by
	int height;		// as above
}TMETRICS;

typedef struct{
	int x;
	int y;
}TPOINTXY;

typedef struct{
	int x;
	int y;
	int data;		// colour, intensity, etc..
}T2POINT;

typedef struct{
	T2POINT	*points;
	int pointsTotal;
	int pointsSpace;
	int dataState;
}TGLYPHPOINTS;

typedef struct{
	unsigned int encoding;	// unicode reference code point
	int	xoffset;			// [bbx] x offset of glyph within frame
	int	yoffset;			// [bbx] y offset of glyph within frame
	int	w;					// [bbx] width of this glyph
	int	h;					// [bbx] height of this glyph
	int	dwidth;				// bdf 'DWIDTH' field
	TRECT box;	
	TGLYPHPOINTS *gp;
	TGLYPHPOINTS *ep[LTR_TOTAL];
}TWCHAR;

typedef struct{
	TWCHAR *glyph;			// glyph source
	TFRAME *to;				// destination frame
	int dx;					// destination point
	int dy;					//
	unsigned int flags;		// print flags (LPRT_nn)
	unsigned int attributes[LTR_TOTAL][16];		// shouldn't really be here
}TLPRINTREGION;

typedef void (*pGlyphRenderFn) (TLPRINTREGION *loc);

typedef struct{
	TLPRINTREGION *loc;
	
	pGlyphRenderFn copy;		// glyph render method
	intptr_t udata;

	int	foreGround;			// ink colour when drawing and printing
	int backGround;			// paper colour when printing and clearing frame or display

	int framesSubmitted;
	int framesRendered;
	int maskLookup[LFRM_TYPES][8];
}TTRENDER;

typedef struct{
	wchar_t	*font;
	wchar_t	*cmap;
}THWDPATHS;

typedef struct{
	TFONT *bmp;			// tga and co.
	TWFONT *bdf;
}TFONTTREES;

typedef struct{
	UTF32 enc;			// code point
	UTF32 uni;			// unicode location
}TTUNIENCMAP;

typedef struct TMAPTABLE{
	int			active;
	int			built;
	int			code;			// cmt_ ID
	char		alias[32];
	wchar_t		file[MaxPath];
	UTF32		*table;			// code points from 0x0000 to 0xFFFF
	TTUNIENCMAP *ltable;		// code points greater than 0xFFFF
	unsigned int ltotal;
	struct TMAPTABLE *next;
}TMAPTABLE;

typedef struct{
	TMAPTABLE *root;
	TMAPTABLE *active;			// pointer to active lookup table
	TMAPTABLE *currentEnumTable;
	int mapCode;				// current decode table id
}TCMAPTREE;

typedef struct{
	unsigned int charRef:1;		// 0 = enabled
	unsigned int charCombine:1;	// 1 = disabled
	unsigned int fill:30;
	int hextodec[128];		// shouldn't be here
	int glyphCount;			// likewise
	
	struct {
		struct {
			int defaultHeight;		// and width
		}ico;
	}image;
}TLIBVARFLAGS;


typedef struct{
	TDRIVER **devlist;		// list of active displays
	unsigned int devcount;	// total activated displays and/or output devices
	int caps[CAPS_TOTAL];	// capability flags
	
	TTRENDER *render;
	THWDPATHS *paths;
	TFONTTREES *fonts;
	TCMAPTREE *cmap;
	TLIBVARFLAGS flags;		// miscellaneous flags and variables
	
	void *sync;
	void *flist;
	void *dtree;
	void *scrtree;
}THWD;

struct TPORTDRIVER{
	char name[lMaxDriverNameLength+1];
	char comment[lMaxDriverCommentLength+1];

	int status;				// driver status: ready, closed or disengaged
	intptr_t *opt;			// storage for individual configurations
	unsigned int optTotal;	// total number of options available for this driver

	int  (*open)		(TPORTDRIVER *pd, int port);
	int  (*close)		(TPORTDRIVER *pd);
	ubyte (*read)		(const int port);
	int  (*write8)		(const int port, const ubyte data);
	int  (*write16)		(const int port, const unsigned short data);
	int  (*write32)		(const int port, const unsigned int data);
	int  (*writeBuffer)	(const int port, void *buffer, size_t tbytes);
	int  (*flush)		(TPORTDRIVER *pd);
	int	 (*setOption)	(TDRIVER *drv, int option, intptr_t *value);
	int  (*getOption)	(TDRIVER *drv, int option, intptr_t *value);
};


struct TDISPLAYDRIVER{
	char name[lMaxDriverNameLength+1];
	char comment[lMaxDriverCommentLength+1];

	int status;						// display status; ready, closed, disengage
	int	width;						// computed display width (virtual)
	int	height;						// computed display height (virtual)
	int	WIDTH;						// actual hardware width
	int	HEIGHT;						// actual hardware height
	int bpp;						// hardware bits per pixel (LFRM_BPP_nn)
	int	clr;						// value used to clear/blank display
	int	port;						// lpt/serial port address
	int currentColumn;				// last column addressed (not always used)
	int currentRow;					// last row or page addressed (not always used)

	THWD *hw;						// root description
	TFRAME *back;					// backbuffer
	TFRAME *temp[LFRM_TYPES+1];		// front buffer - copy of frame area which is sent to display
	intptr_t tmpGrpId;
	intptr_t *opt;					// storage list for individual display configuration(s) or settings
	int optTotal;					// number of options available for this display

	int	(*open)			(TDRIVER *drv);
	int	(*close)		(TDRIVER *drv);
	int	(*clear)		(TDRIVER *drv);
	int	(*refresh)		(TDRIVER *drv, TFRAME *frm);
	int	(*refreshArea)	(TDRIVER *drv, TFRAME *frm, int left, int top, int right, int bottom);
	int (*setOption)	(TDRIVER *drv, int option, intptr_t *value);
	int (*getOption)	(TDRIVER *drv, int option, intptr_t *value);
};

typedef struct {
	// set a pixel. x and y is bound checked
	void (*set)		(const TFRAME *frm, const int x, const int y, const int colour);

	// return pixel value. x and y is bound checked
	int (*get)		(const TFRAME *frm, const int x, const int y);

	// set a pixel without checking if x,y location lies within frame boundary
	void (*set_NB)	(const TFRAME *frm, const int x, const int y, const int colour);

	// return pixel value without checking if x,y location lies within frame boundary
	int (*get_NB)	(const TFRAME *frm, const int x, const int y);

	// set floating point colour for each component from 0 to 1
	void (*setf)	(const TFRAME *frm, const int x, const int y, const float r, const float g, const float b);

	// return floating point RGB value for each component
	int (*getf)		(const TFRAME *frm, const int x, const int y, float *r, float *g, float *b);

	// return address of pixel. note: frame format LFRM_BPP_1 may return the same address for multiple pixels
	void *(*getAddr)(const TFRAME *frm, const int x, const int y);

	// set a pixel without checking if pixel x on row 'row' lies within frame boundary
	//row = y * pitch
	void (*set_NBr)	(const TFRAME *frm, const int x, const int row, const int colour);

	// return pixel value without checking if pixel x on row 'row' lies within frame boundary
	//row = y * pitch
	int (*get_NBr)	(const TFRAME *frm, const int x, const int row);
}TPIXELPRIMITVES;

struct TFRAME{
	THWD	*hw;			// root description
	int		width;			// frame width in pixels
	int		height;			// frame height in pixels
	int 	pitch;			// width of one row, in bytes
	ubyte	*pixels;		// pixel data
	size_t	frameSize;		// size of pixel buffer in bytes, includes padding
	ubyte	bpp;			// idx to bits per pixel (LFRM_BPP_xx)
	ubyte	style;			// pixel operation mode (LSP_xxx)
	int		groupId;
	void	*udata_ptr;		// user storage, pointer
	int 	udata_int;		// user storage, int
	TPIXELPRIMITVES *pops;	// pixel access methods
};

typedef struct{
	int x1;
	int y1;
	int x2;
	int y2;
}TLPOINTEX;

typedef struct{
	int bx1;	// bounding region
	int by1;	// text will not be rendered outside of this box
	int bx2;	//
	int by2;

	int sx;		// start point
	int sy;		// rendering begins at this point and wihin frame boundary [defined above]
	int ex;		// end point
	int ey;		// last point plotted on render completion
}TLPRINTR;

typedef struct {
	THWD *hw;
	TFRAME *srcFrm;			// original image
	TFRAME *desFrm;			// copy buffer
	TFRAME *clientFrm;		// actual clients distination buffer
	TLPOINTEX *srcRect;
	TLPOINTEX *desRect;

	int flags;
	int dir;			// scroll direction
	int startPos;		// destination image; starting position
	int pos;			// destination image; current offset
	int loopGapWidth;	// pixel [width] space between loops
}TLSCROLLEX;

struct TFONT{
	unsigned int built:1;		// font is built
	unsigned int invert:1;		// texture is inverted, set to invert font
	unsigned int autowidth:1;	// automatically calculate cell width (charW)
	unsigned int autoheight:1;	// automatically calculate cell height (charH)
	unsigned int padding:28;

	int id;				// font id
	int charW;			// maximum width of all glyphs
	int charH;			// mamimum height of all glyphs
	int charsPerRow;	// chracters per row
	int rowsPerImage;	// above rows per image
	int xoffset;		// horiztonal glyph postion offset
	int yoffset;		// vertical glyph postion offset
	int choffset;		// index value added to character encoding
	int charSpace;		// space between adjacent glyphs
	int lineSpace;		// number of vertical pixels to skip between wrapped lines

	TFRAME *f;			// bitmap container
	TRECT c[256];		// storage for characters 1-255
	wchar_t file[MaxPath];
	int imageType;		// BMP, PGM, PNG or TGA
	TFONT *next;
	THWD *hw;
};

typedef struct {
	int x;
	int y;
	int z1;
	int z2;
	//uint64_t time;
	double time;
	double dt;
	unsigned int count;	// this is n'th report
	unsigned char pen;
	unsigned char pressure;
	unsigned int id;
}TTOUCHCOORD;
#define TTOUCHCOORD30 1

typedef struct{
	UTF32 *uslist;		// list of code points requested but found to be missing from font
	int total;			// number of chars in list (should be equal to (uslist size/sizeof(UTF32))
}TUSC;

typedef struct{
	fpos_t offset;
	unsigned int encoding;
}TCHAROFFSET;

struct TWFONT {
	int built;					// is font allocated and built
	int FontID;
	unsigned int flags;

	struct {
		int enabled;
		int chainId;			// if this font has an active render filter chain attached, then this is the reference ID
	}filter;

	wchar_t file[MaxPath];		// file path
	char fontName[128];			// font descryption information
	char CharsetRegistry[128];	// standard for which this font complies with
	char FamilyName[64];		// font belongs to this family

	unsigned int DefaultChar;	// glyph to use in place of missing glyph (in font)
	int CharsBuilt;				// actual number of glyphs built
	int GlyphsInFont;			// reported number of glyphs in font (BDF 'CHARS' field)
	int QuadWidth;				// FBB, quad width = max width of all char's
	int QuadHeight;				// FBB, quad height = max height of all char's
	int QuadXOffset;			// FBB - FONTBOUNDINGBOX
	int QuadYOffset;			// FBB
	int fontAscent;				// BDF 'FONT_ASCENT' field
	int fontDescent;			// BDF 'FONT_DESCENT' field
	char spacing;				// BDF 'SPACING' field
	int PixelSize;				// BDF 'PIXEL_SIZE' field describing maximum rows, ie height
	int CharSpace;				// total number of pixels to skip between horizontal character rendering
	int LineSpace;				// number of vertical pixels to skip between wrapped lines
	int spaceSpacing;			// pixels to add/subtract from zero width glyphs (eg; ' '). is always 0 by default

	void *fp;					// file handle used between calls
	TUSC uscl;					// list of requested but unsupported code points

	unsigned int maxCharIdx;	// final character index (max chars)
	TCHAROFFSET *chrOffset;
	unsigned int coTotal;
	
	TWCHAR **chr;				// glyphs' container
	THWD *hw;
	
	TWFONT *next;
};

typedef struct {
	TFONT *font;				// image file (bitmap) font
	TWFONT *wfont;				// bdf wide font

	int total;					// total fonts registered
	int fi;						// font index, a counter really
	int id;						// registered font id
	THWD *hw;
}TENUMFONT;

typedef struct{
	unsigned int index;
	int	total;
	int type;	// display or port driver
	char name[lMaxDriverNameLength+1];
	char comment[lMaxDriverCommentLength+1];
	THWD *hw;
}TREGDRV;


#endif


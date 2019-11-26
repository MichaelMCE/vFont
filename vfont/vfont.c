

// A Vector font libarary beased upon the Hershey font set
// 
// Michael McElligott
// okio@users.sourceforge.net
// https://youtu.be/T0WgGcm7ujM

//  Copyright (c) 2005-2017  Michael McElligott
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






#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>

#include "vfont.h"





static inline void drawBrushBitmap (vfont_t *ctx, const int x, const int y, const uint16_t colour)
{
	drawBitmap(&ctx->brush.image, x, y, colour);
}
	
static inline void drawBrush (vfont_t *ctx, float xc, float yc, const float radius, const uint16_t colour)
{
	
	if (ctx->brush.type == BRUSH_DISK){
		drawCircleFilled(xc, yc, radius, colour);

	}else if (ctx->brush.type == BRUSH_SQUARE_FILLED){
		int d = (int)radius>>1;
		drawRectangleFilled(xc-d, yc-d, xc+d, yc+d, colour);

	}else if (ctx->brush.type == BRUSH_SQUARE){
		int d = (int)radius>>1;
		drawRectangle(xc-d, yc-d, xc+d, yc+d, colour);

	}else if (ctx->brush.type == BRUSH_TRIANGLE_FILLED){
		int d = (int)radius>>1;
		drawTriangleFilled(xc-d, yc+d, xc, yc-d, xc+d, yc+d, colour);
		
	}else if (ctx->brush.type == BRUSH_TRIANGLE){
		int d = (int)radius>>1;
		drawTriangle(xc-d, yc+d, xc, yc-d, xc+d, yc+d, colour);

	}else if (ctx->brush.type == BRUSH_CIRCLE){
		drawCircle(xc, yc, radius, colour);
	
	}else if (ctx->brush.type == BRUSH_STROKE_1){
		int d = (int)radius>>1;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);		// slope up
		xc++;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);
		
	}else if (ctx->brush.type == BRUSH_STROKE_2){
		int d = (int)radius>>1;
		drawLine(xc-d, yc-d, xc+d, yc+d, colour);		// slope down
		xc++;
		drawLine(xc-d, yc-d, xc+d, yc+d, colour);

	}else if (ctx->brush.type == BRUSH_STROKE_3){
		int d = (int)radius>>1;
		drawLine(xc-d, yc, xc+d, yc, colour);			// horizontal
		yc++;
		drawLine(xc-d, yc, xc+d, yc, colour);

	}else if (ctx->brush.type == BRUSH_STROKE_4){
		int d = (int)radius>>1;
		drawLine(xc, yc-d, xc, yc+d, colour);			// vertical
		xc++;
		drawLine(xc, yc-d, xc, yc+d, colour);

	}else if (ctx->brush.type == BRUSH_STROKE_5){
		int d = (int)radius>>1;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);		// forward slope up with smaller siblings either side
		d = radius*0.3f;
		xc++;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);
		xc -= 2.0f;
		//yc -= 2.0f;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);

	}else if (ctx->brush.type == BRUSH_STROKE_6){
		int d = (int)radius>>1;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);		// BRUSH_STROKE_5 but thicker
		xc++;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);
		d = radius*0.3f;
		xc++;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);		// right side
		xc -= 2.0f; yc -= 1.0f;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);		// left side
				
	}else if (ctx->brush.type == BRUSH_STROKE_7){
		int d = (int)radius>>1;
		//drawLine(xc-d, yc+d, xc+d, yc-d, colour);
		
		int d2 = d + 2;
		drawLine(xc-d2, yc+d2, xc-d, yc+d, colour);
		drawLine((xc-d2)-1, yc+d2, (xc-d)-1, yc+d, colour);
		drawLine(xc+d, yc-d, xc+d2, yc-d2, colour);
		drawLine(xc+d+1, yc-d, xc+d2+1, yc-d2, colour);

		drawLine(xc+d2, yc+d2, xc+d, yc+d, colour);
		drawLine(xc+d2+1, yc+d2, xc+d+1, yc+d, colour);
		drawLine(xc-d, yc-d, xc-d2, yc-d2, colour);
		drawLine((xc-d)-1, yc-d, (xc-d2)-1, yc-d2, colour);

		
	}else if (ctx->brush.type == BRUSH_STAR){
		int d = (int)radius>>1;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);		// slope up
		drawLine(xc-d, yc-d, xc+d, yc+d, colour);		// slope down
		drawLine(xc-d, yc, xc+d, yc, colour);			// horizontal
		drawLine(xc, yc-d, xc, yc+d, colour);			// vertical
		
	}else if (ctx->brush.type == BRUSH_X){
		int d = (int)radius>>1;
		drawLine(xc-d, yc+d, xc+d, yc-d, colour);		// slope up
		drawLine(xc-d, yc-d, xc+d, yc+d, colour);		// slope down
		
	}else if (ctx->brush.type == BRUSH_PLUS){
		int d = (int)radius>>1;
		drawLine(xc-d, yc, xc+d, yc, colour);			// horizontal
		drawLine(xc, yc-d, xc, yc+d, colour);			// vertical

	}else if (ctx->brush.type == BRUSH_CARET){
		int d = (int)radius>>1;							// ^ (XOR operator)
		drawLine(xc-d, yc+d, xc, yc-d, colour);			// left
		drawLine(xc, yc-d, xc+d, yc+d, colour);			// right
		
	}else if (ctx->brush.type == BRUSH_BITMAP){
		drawBrushBitmap(ctx, xc, yc, colour);
		
	}else{	// BRUSH_POINT
		drawPixel(xc, yc, colour);
	}
}

static inline float distance (const float x1, const float y1, const float x2, const float y2)
{
	return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//check if point2 is between point1 and point3 (the 3 points should be on the same line)
static inline int isBetween (const float x1, const float y1, const float x2, const float y2, const float x3, const float y3)
{
	return ((int)(x1 - x2) * (int)(x3 - x2) <= 0) && ((int)(y1 - y2) * (int)(y3 - y2) <= 0);
}

static inline float dot (float x1, float y1, float x2, float y2)
{
    return x1 * x2 + y1 * y2;
}

static inline void drawBrushVector (vfont_t *ctx, const float x1, const float y1, const float x2, const float y2, const uint16_t colour)
{
	if (ctx->brush.size > 1.0f){
		float i = 0.0f;
		float x = x1;
		float y = y1;
		const float d = distance(x1, y1, x2, y2);
		const float dx = x2 - x1;
		const float dy = y2 - y1;
		const float bmul = ctx->brush.advanceMult;
		const float bmulX = bmul * dx / d;
		const float bmulY = bmul * dy / d;
		
		while (distance(x, y, x2, y2) > bmul && isBetween(x1, y1, x, y, x2, y2)){
			i += 1.0f;
			x = (x1 + i * bmulX);
			y = (y1 + i * bmulY);
      
			drawBrush(ctx, x, y, ctx->brush.size / 2.0f, colour);
		}
	}else{
#if 0
		// cross product
		float x = (y1 * 1.0f) - (1.0f * y2);
		float y = (1.0f * x2) - (x1 * 1.0f);
		const float length = sqrtf((x * x) + (y * y));
	
		if (length < 2.0f) return;
		// set thickness;
		x *= 3.5f * (1.0f/length);
		y *= 3.5f * (1.0f/length);
		
		float xh = (x / 2.0f);	// center path by precomputing/using half thickness per side
		float yh = (y / 2.0f);

		drawTriangleFilled(x1+xh, y1+yh, x1-xh, y1-yh, x2+xh, y2+yh, colour);
		drawTriangleFilled(x1-xh, y1-yh, x2+xh, y2+yh, x2-xh, y2-yh, colour);
		
		//drawCircleFilled(x1, y1, 10.0f, colour);	// path start
		
#else
		drawLine(x1, y1, x2, y2, colour);
#endif	
	}
}

static inline void rotateZ (rotate_t *rot, const float x, const float y, float *xr, float *yr)
{
	*xr = x * rot->cos - y * rot->sin;
	*yr = x * rot->sin + y * rot->cos;
}

static inline void drawVector (vfont_t *ctx, float x1, float y1, float x2, float y2)
{

	if (ctx->renderOp == RENDEROP_NONE){
		drawBrushVector(ctx, x1, y1, x2, y2, ctx->brush.colour);
		return;

	}else{
		// transform to 0
		x1 -= ctx->x; x2 -= ctx->x;
		y1 -= ctx->y; y2 -= ctx->y;
	}
	

	if (ctx->renderOp&RENDEROP_SHEAR_X){
		x1 += y1 * ctx->shear.tan;
		x2 += y2 * ctx->shear.tan;
	}
	
	if (ctx->renderOp&RENDEROP_SHEAR_Y){
		y1 = (x1 * ctx->shear.sin + y1 * ctx->shear.cos) + 1.0f;
		y2 = (x2 * ctx->shear.sin + y2 * ctx->shear.cos) + 1.0f;
	}

	
	if (ctx->renderOp&RENDEROP_ROTATE_GLYPHS){
		// undo string transform
		x1 += ctx->x; x2 += ctx->x;
		y1 += ctx->y; y2 += ctx->y;

		// apply glyph transform
		x1 -= ctx->pos.x; x2 -= ctx->pos.x;
		y1 -= ctx->pos.y; y2 -= ctx->pos.y;

		rotateZ(&ctx->rotate.glyph, x1, y1, &x1, &y1);
		rotateZ(&ctx->rotate.glyph, x2, y2, &x2, &y2);

		x1 += ctx->pos.x - ctx->x; x2 += ctx->pos.x - ctx->x;
		y1 += ctx->pos.y - ctx->y; y2 += ctx->pos.y - ctx->y;
	}
	
	if (ctx->renderOp&RENDEROP_ROTATE_STRING){
		rotateZ(&ctx->rotate.string, x1, y1, &x1, &y1);
		rotateZ(&ctx->rotate.string, x2, y2, &x2, &y2);
	}

	//  transform back
	x1 += ctx->x; x2 += ctx->x;
	y1 += ctx->y; y2 += ctx->y;
	
	// now draw the brush
	drawBrushVector(ctx, x1, y1, x2, y2, ctx->brush.colour);

}

static inline float char2float (vfont_t *ctx, const uint8_t c)
{
	return ctx->scale.glyph * (float)(c - 'R');
}

// returns horizontal glyph advance
static inline float drawGlyph (vfont_t *ctx, const hfont_t *font, const uint16_t c)
{

	if (c >= font->glyphCount) return 0.0f;
	
	const uint8_t *hc = (uint8_t*)font->glyphs[c];
	const float lm = char2float(ctx, *hc++) * fabsf(ctx->scale.horizontal);
	const float rm = char2float(ctx, *hc++) * fabsf(ctx->scale.horizontal);
	
	ctx->pos.x -= lm;

	float x1 = 0.0f;
	float y1 = 0.0f;
	float x2, y2;
	int newPath = 1;

	while (*hc){
		if (*hc == ' '){
			hc++;
			newPath = 1;

		}else{
			const float x = char2float(ctx, *hc++) * ctx->scale.horizontal;
			const float y = char2float(ctx, *hc++) * ctx->scale.vertical;

			if (newPath){
				newPath = 0;
				x1 = ctx->pos.x + x;
				y1 = ctx->pos.y + y;

				//drawCircleFilled(x1, y1, 5, COLOUR_PAL_BLUE);	// path start
			}else{
				x2 = ctx->pos.x + x;
				y2 = ctx->pos.y + y;
				
				drawVector(ctx, x1, y1, x2, y2);
				x1 = x2;
				y1 = y2;
				
				//if (*hc == ' ' || !*hc) drawCircleFilled(x1, y1, 5, COLOUR_PAL_BLUE); // path end
			}
		}
	}

	ctx->pos.x += rm + ctx->xpad;
	return (rm - lm) + ctx->xpad;
}

// returns scaled glyph stride 
float getCharMetrics (vfont_t *ctx, const hfont_t *font, const uint16_t c, float *adv, box_t *box)
{

	if (c >= font->glyphCount) return 0.0f;

	const uint8_t *hc = (uint8_t*)font->glyphs[c];
	const float lm = char2float(ctx, *hc++) * fabsf(ctx->scale.horizontal);
	const float rm = char2float(ctx, *hc++) * fabsf(ctx->scale.horizontal);

	float startX;
	if (adv)
		startX = *adv - lm;
	else
		startX = 0.0f - lm;
	
	
	float startY = 0.0f;
	float miny = 999.0f;
	float maxy = -999.0f;
	float minx = 999.0f;
	float maxx = -999.0f;
	
	
	while (*hc){
		if (*hc == ' '){
			hc++;

		}else{
			float x = char2float(ctx, *hc++);
			if (x > maxx) maxx = x;
			if (x < minx) minx = x;
			
			float y = char2float(ctx, *hc++);
			if (y > maxy) maxy = y;
			if (y < miny) miny = y;
		}
	}
	
	minx *= fabsf(ctx->scale.horizontal);
	maxx *= fabsf(ctx->scale.horizontal);
	miny *= fabsf(ctx->scale.vertical);
	maxy *= fabsf(ctx->scale.vertical);
		
	float brushSize = ctx->brush.size / 2.0f;
	box->x1 = (startX + minx) - brushSize;
	box->y1 = (startY + miny) - brushSize;
	box->x2 = (startX + maxx) + brushSize;
	box->y2 = (startY + maxy) + brushSize;

	if (adv) *adv = startX + rm + ctx->xpad;
	return startX + rm + ctx->xpad;
}

void getGlyphMetrics (vfont_t *ctx, const uint16_t c, int *w, int *h)
{
	box_t box = {0};
	getCharMetrics(ctx, ctx->font, c-32, NULL, &box);

	if (w) *w = ((box.x2 - box.x1) + 1.0f) + 0.5f;
	if (h) *h = ((box.y2 - box.y1) + 1.0f) + 0.5f;
}

void getStringMetrics (vfont_t *ctx, const char *text, box_t *box)
{
#if 0
	int x = ctx->x;
	int y = ctx->y;
#endif

	float miny = 999.0f;
	float maxy = -999.0f;
	float minx = 999.0f;
	float maxx = -999.0f;
	float adv = 0.0f;
	
	while (*text){
		memset(box, 0, sizeof(box_t));
		getCharMetrics(ctx, ctx->font, ((uint16_t)(*text++))-32, &adv, box);
#if 0
		drawRectangle(x+box->x1, y+box->y1, x+box->x2, y+box->y2, COLOUR_PAL_BLUE);
#endif
		
		if (box->x1 < minx) minx = box->x1;
		if (box->x2 > maxx) maxx = box->x2;
		if (box->y1 < miny) miny = box->y1;
		if (box->y2 > maxy) maxy = box->y2;
	}

	box->x1 = minx;
	box->y1 = miny;
	box->x2 = maxx;
	box->y2 = maxy;
}

void drawString (vfont_t *ctx, const char *text, const int x, const int y)
{
	ctx->x = ctx->pos.x = x;
	ctx->y = ctx->pos.y = y;
	
	
	while (*text)
		drawGlyph(ctx, ctx->font, ((uint16_t)(*text++))-32);
}

int setBrushBitmap (vfont_t *ctx, const void *bitmap, const uint8_t width, const uint8_t height)
{
	ctx->brush.image.pixels = (uint8_t*)bitmap;
	ctx->brush.image.width = width;
	ctx->brush.image.height = height;
	
	return (bitmap && width && height);
}

void setFont (vfont_t *ctx, const hfont_t *font)
{
	if (font)
		ctx->font = font;
}

const hfont_t *getFont (vfont_t *ctx)
{
	return ctx->font;
}

int setBrush (vfont_t *ctx, const int brush)
{
	if (brush < BRUSH_TOTAL){
		int old = ctx->brush.type;
		ctx->brush.type = brush;
		return old;
	}
	return ctx->brush.type;
}


// calculate glyph stride
static inline void brushCalcAdvance (vfont_t *ctx)
{
	ctx->brush.advanceMult = (ctx->brush.size * ctx->brush.step) / 100.0f;
}

float setBrushSize (vfont_t *ctx, const float size)
{
	if (size >= 0.5f){
		float old = ctx->brush.size;
		ctx->brush.size = size;
		brushCalcAdvance(ctx);
		return old;
	}
	return ctx->brush.size;
}

// 0.5 to 100.0
void setBrushStep (vfont_t *ctx, const float step)
{
	if (step >= 0.5f){
		ctx->brush.step = step;
		brushCalcAdvance(ctx);
	}
}

float getBrushStep (vfont_t *ctx)
{
	return ctx->brush.step;
}


void setGlyphScale (vfont_t *ctx, const float scale)
{
	if (scale >= 0.1f)
		ctx->scale.glyph = scale;
}

float getGlyphScale (vfont_t *ctx)
{
	return ctx->scale.glyph;
}

// Extra space added to every glyph. (can be 0 or minus)
void setGlyphPadding (vfont_t *ctx, const float pad)
{
	ctx->xpad = pad;
}

float getGlyphPadding (vfont_t *ctx)
{
	return ctx->xpad;
}

uint16_t setBrushColour (vfont_t *ctx, const uint16_t colour)
{
	uint16_t old = ctx->brush.colour;
	ctx->brush.colour = colour;
	return old;
}


uint16_t getBrushColour (vfont_t *ctx)
{
	return ctx->brush.colour;
}

void setRenderFilter (vfont_t *ctx, const uint32_t op)
{
	ctx->renderOp = op&0xFFFF;
}

uint32_t getRenderFilter (vfont_t *ctx)
{
	return ctx->renderOp;
}

// when rotation from horizontal, when enabled via setRenderFilter()
void setRotationAngle (vfont_t *ctx, const float rotGlyph, const float rotString)
{
	const float oldG = ctx->rotate.glyph.angle;
	ctx->rotate.glyph.angle = fmodf(rotGlyph, 360.0f);

	if (oldG != ctx->rotate.glyph.angle){
		ctx->rotate.glyph.cos = cosf(DEG2RAD(ctx->rotate.glyph.angle));
		ctx->rotate.glyph.sin = sinf(DEG2RAD(ctx->rotate.glyph.angle));
	}
	
	const float oldS = ctx->rotate.string.angle;
	ctx->rotate.string.angle = fmodf(rotString, 360.0f);
	
	if (oldS != ctx->rotate.string.angle){
		ctx->rotate.string.cos = cosf(DEG2RAD(ctx->rotate.string.angle));
		ctx->rotate.string.sin = sinf(DEG2RAD(ctx->rotate.string.angle));
	}
	
}

void setShearAngle (vfont_t *ctx, const float shrX, const float shrY)
{
	ctx->shear.angleX = shrX;
	ctx->shear.angleY = shrY;
	
	ctx->shear.tan = -tanf(DEG2RAD(ctx->shear.angleX) /*/ 2.0f*/);
	ctx->shear.cos = cosf(DEG2RAD(ctx->shear.angleY));
	ctx->shear.sin = sinf(DEG2RAD(ctx->shear.angleY));
}


void setAspect (vfont_t *ctx, const float hori, const float vert)
{
	if (hori >= 0.05f || hori <= -0.05f)
		ctx->scale.horizontal = hori;
	
	if (vert >= 0.05f || vert <= -0.05f)
		ctx->scale.vertical = vert;
}

void vfontInitialise (vfont_t *ctx)
{
	memset(ctx, 0, sizeof(*ctx));

	setAspect(ctx, 1.0f, 1.0f);
	setGlyphPadding(ctx, -1.0f);
	setGlyphScale(ctx, 1.0f);
	setBrush(ctx, BRUSH_DISK);
	setBrushSize(ctx, 1.0f);
	setBrushStep(ctx, 1.0f);
	setBrushColour(ctx, COLOUR_RED);
	setRenderFilter(ctx, RENDEROP_NONE);

}

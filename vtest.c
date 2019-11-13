

// A Vector font libarary beased upon the Hershey font set
// 
// Michael McElligott
// okio@users.sourceforge.net
// https://youtu.be/T0WgGcm7ujM

//  Copyright (c) Michael McElligott
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

#include <mylcd.h>
#include <demos.h>

#include "vfont/vfont.h"



#if USBD480
static USBHost myusb;
static USBHub hub1(myusb);
static USBD480Display usbd480(myusb);
#endif

// i'm a C guy at heart..
static uint16_t palette[256];


#if USBD480
enum _pal {
	COLOUR_PAL_BLACK,

	COLOUR_PAL_RED,
	COLOUR_PAL_GREEN,
	COLOUR_PAL_BLUE,
	
	COLOUR_PAL_DARKGREEN,   
	COLOUR_PAL_DARKGREY,	
	COLOUR_PAL_HOMER,	    
	COLOUR_PAL_REDISH,	    
	COLOUR_PAL_MAGENTA,
	COLOUR_PAL_CREAM,
	COLOUR_PAL_CYAN,
	COLOUR_PAL_YELLOW,
	COLOUR_PAL_ORANGE,
	COLOUR_PAL_BLUE_SEA_TINT,
	COLOUR_PAL_GREEN_TINT,	
	COLOUR_PAL_PURPLE_GLOW,	
	COLOUR_PAL_AQUA,
	COLOUR_PAL_GRAY,
	COLOUR_PAL_HOVER,
	COLOUR_PAL_TASKBARFR,
	COLOUR_PAL_TASKBARBK,
	COLOUR_PAL_SOFTBLUE,	
	COLOUR_PAL_WHITE,
	COLOUR_PAL_TOTAL
};
#endif


static inline void clearFrame (const uint8_t palIdx)
{
	memset(renderBuffer, palIdx, VWIDTH * VHEIGHT);
}

static inline void clearDisplay ()
{
	//usbd480.fillScreen(COLOUR_CREAM);
	lClearFrameClr(frame, COLOUR_CREAM);
}

static inline void updateDisplay ()
{
#if USBD480
	usbd480.drawScreenArea(renderBuffer, VWIDTH, VHEIGHT, 0, 0);
#endif

	uint16_t *pixels = (uint16_t*)frame->pixels;
	
	for (int i = 0; i < VWIDTH * VHEIGHT; i++)
		pixels[i] = palette[(uint8_t)renderBuffer[i]];
	
	lUpdate(hw, pixels, VWIDTH*VHEIGHT*2);
}

#if USBD480
int driver_callback (uint32_t msg, intptr_t *value1, uint32_t value2)
{
	if (msg == USBD_MSG_DISPLAYREADY){
		//delay(50);
#if 0
		char name[DEVICE_NAMELENGTH+1];
		char serial[DEVICE_SERIALLENGTH+1];
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t version = 0;
	
		usbd480.getDeviceDetails(name, &width, &height, &version, serial);
		Serial.println(name);
		Serial.println(serial);
		Serial.printf("%i %i %i\r\n", width, height, version);
#endif
		clearDisplay();
		doTests = 1;
	}
	
	return 1;
}


#endif



static inline void paletteSet (const uint8_t idx, const uint16_t colour)
{
	palette[idx] = colour;
}

static inline void buildPalette ()
{
	paletteSet(COLOUR_PAL_BLACK,		COLOUR_BLACK);
	paletteSet(COLOUR_PAL_RED,			COLOUR_RED);
	paletteSet(COLOUR_PAL_GREEN,		COLOUR_GREEN);
	paletteSet(COLOUR_PAL_BLUE,			COLOUR_BLUE);
	paletteSet(COLOUR_PAL_WHITE,		COLOUR_WHITE);
	paletteSet(COLOUR_PAL_CREAM,		COLOUR_CREAM);
	paletteSet(COLOUR_PAL_DARKGREEN,	COLOUR_24TO16(0x00AA55));
	paletteSet(COLOUR_PAL_DARKGREY,		COLOUR_24TO16(0x444444));
	paletteSet(COLOUR_PAL_HOMER,		COLOUR_24TO16(0xFFBF33));
	paletteSet(COLOUR_PAL_REDISH,		COLOUR_24TO16(0xFF0045));
	paletteSet(COLOUR_PAL_MAGENTA,		COLOUR_RED|COLOUR_BLUE);
	paletteSet(COLOUR_PAL_CYAN,			COLOUR_CYAN);
	paletteSet(COLOUR_PAL_AQUA,			COLOUR_AQUA);
	paletteSet(COLOUR_PAL_YELLOW,		COLOUR_YELLOW);
	paletteSet(COLOUR_PAL_ORANGE,		COLOUR_ORANGE);
	
	paletteSet(COLOUR_PAL_BLUE_SEA_TINT,COLOUR_BLUE_SEA_TINT);
	paletteSet(COLOUR_PAL_GREEN_TINT,	COLOUR_GREEN_TINT); 
	paletteSet(COLOUR_PAL_PURPLE_GLOW,	COLOUR_PURPLE_GLOW);
	
	
	paletteSet(COLOUR_PAL_GRAY,			COLOUR_GRAY);
	paletteSet(COLOUR_PAL_HOVER,		COLOUR_HOVER);
	paletteSet(COLOUR_PAL_TASKBARFR,	COLOUR_TASKBARFR);
	paletteSet(COLOUR_PAL_TASKBARBK,	COLOUR_TASKBARBK);
	paletteSet(COLOUR_PAL_SOFTBLUE,		COLOUR_SOFTBLUE);



	
	paletteSet(COLOUR_PAL_TOTAL-1,		COLOUR_WHITE);
}

void setup (vfont_t *ctx)
{

#if USBD480
	while (!Serial) ; // wait for Arduino Serial Monitor
	
	Serial.println("vFont");
	myusb.begin();
	usbd480.setCallbackFunc(driver_callback);
	delay(25);
#endif
	buildPalette();
	
	vfontInitialise(ctx);
	setFont(ctx, &gothiceng);
	//setFont(ctx, &futural);
	
	setGlyphPadding(ctx, -2.5f);
	setGlyphScale(ctx, 2.0f);
	setBrushColour(ctx, COLOUR_PAL_RED);
	setRenderFilter(ctx, RENDEROP_NONE);
	
	//setRenderFilter(ctx, RENDEROP_ROTATE_GLYPHS | RENDEROP_ROTATE_STRING);
	setRotationAngle(ctx, -45.0f, 55.0f);
	setShearAngle(ctx, 35.0f, 0.0f);
	setBrushBitmap(ctx, smiley16x16, 16, 16);

}



void doTests (vfont_t *ctx)
{
	char tbuffer[32];

	clearFrame(COLOUR_PAL_CREAM);
	//uint32_t t2 = micros();
	setBrush(ctx, BRUSH_DISK);
	setAspect(ctx, 1.0f, 1.0f);
	
#if 0
	
	//static float angle = 0.0f;
	//setRenderFilter(ctx, RENDEROP_ROTATE_GLYPHS | RENDEROP_ROTATE_STRING);
	//angle = fmodf(angle+1.0f, 360.0f);
	//setRotationAngle(ctx, -angle, angle);
	
	int x = 50;
	int y = 100;
	
	setFont(ctx, &gothiceng);
	setGlyphScale(ctx, 4.0f);
	setRenderFilter(ctx, RENDEROP_NONE);
	setAspect(ctx, 1.0f, 1.0f);
	setBrush(ctx, BRUSH_DISK);
	setBrushStep(ctx, 5.0f);
	setBrushSize(ctx, 60.0f);
	setBrushColour(ctx, COLOUR_PAL_DARKGREEN);
	drawString(ctx, "Gothic", x, y);
	
	setBrushStep(ctx, 3.0f);
	setBrushSize(ctx, 10.0f);
	setBrushColour(ctx, COLOUR_PAL_DARKGREY);
	drawString(ctx, "Gothic", x, y);

	setBrushStep(ctx, 1.0f);
	setBrushSize(ctx, 3.0f);
	setBrushColour(ctx, COLOUR_PAL_HOMER);
	drawString(ctx, "Gothic", x, y);
	
	setBrushColour(ctx, COLOUR_PAL_DARKGREY);
	setBrushSize(ctx, 1.0f);
	drawString(ctx, "Gothic", x, y);


	setFont(ctx, &scriptc);
	setGlyphScale(ctx, 3.0f);
	setBrush(ctx, BRUSH_STROKE_6);
	setBrushStep(ctx, 1.0f);
	setBrushSize(ctx, 13.0f);
	setBrushColour(ctx, COLOUR_PAL_REDISH);
	drawString(ctx, "Script", 20, (VHEIGHT>>1)+65);
	
	
	setFont(ctx, &scripts);
	setGlyphScale(ctx, 1.5f);
	setBrush(ctx, BRUSH_STROKE_1);
	setBrushStep(ctx, 1.0f);
	setBrushSize(ctx, 5.0f);
	setBrushColour(ctx, COLOUR_PAL_MAGENTA);
	drawString(ctx, "Script", (VWIDTH>>1)+20, (VHEIGHT>>1)+80);
	
	
	setFont(ctx, &futural);
	setGlyphScale(ctx, 3.7f);
	setBrush(ctx, BRUSH_BITMAP);
	setBrushStep(ctx, 75.0f);
	setBrushSize(ctx, 10.0f);
	setBrushColour(ctx, COLOUR_PAL_BLUE);
	drawString(ctx, "B", VWIDTH-90, VHEIGHT-60);
#endif

#if 1
	int x = 50;
	int y = 60;
	
	setFont(ctx, &timesr);
	setBrush(ctx, BRUSH_STROKE_1);
	setGlyphScale(ctx, 3.0f);
	setBrushStep(ctx, 2.0f);
	setBrushSize(ctx, 17.0f);
	setBrushColour(ctx, COLOUR_PAL_REDISH);
	drawString(ctx, "Stroked", x, y);


	setFont(ctx, &timesrb);
	setBrush(ctx, BRUSH_TRIANGLE_FILLED);
	setGlyphScale(ctx, 3.0f);
	setBrushStep(ctx, 2.0f);
	setBrushSize(ctx, 15.0f);
	setBrushColour(ctx, COLOUR_PAL_DARKGREEN);
	drawString(ctx, "again", 10, y+70);


	setFont(ctx, &timesrb);
	setBrush(ctx, BRUSH_SQUARE_FILLED);
	setGlyphScale(ctx, 3.0f);
	setBrushStep(ctx, 2.0f);
	setBrushSize(ctx, 15.0f);
	setBrushColour(ctx, COLOUR_PAL_DARKGREY);
	drawString(ctx, "and..", (VWIDTH>>1)-50, y+135);


	setFont(ctx, &timesi);
	setGlyphScale(ctx, 2.0f);
	setBrushStep(ctx, 1.0f);
	setBrushSize(ctx, 1.0f);		// brushsize of of 1.0 or less defaults to polyline
	setBrushColour(ctx, COLOUR_PAL_MAGENTA);
	drawString(ctx, "line", 10, VHEIGHT-50);
	
#endif


#if 1
	static float angle = 0.0f;
	setGlyphScale(ctx, 1.0f);
	setBrushStep(ctx, 1.0f);
	setBrushSize(ctx, 1.0f);
	setFont(ctx, &gothiceng);
	
	angle = fmodf(angle+1.0f, 360.0f);
	setRotationAngle(ctx, -angle, angle);

	setRenderFilter(ctx, RENDEROP_ROTATE_STRING);
	setBrushColour(ctx, COLOUR_PAL_REDISH);
	drawString(ctx, "Vector Fonts", (VWIDTH>>1)-30, (VHEIGHT>>1)-30);
	
	setRenderFilter(ctx, RENDEROP_ROTATE_GLYPHS | RENDEROP_ROTATE_STRING);
	setBrushColour(ctx, COLOUR_PAL_DARKGREEN);
	drawString(ctx, "Teensy3.6", (VWIDTH>>1)+20, (VHEIGHT>>1)+20);

	setFont(ctx, &futuram);
	setRenderFilter(ctx, RENDEROP_SHEAR_X);
	setShearAngle(ctx, angle, 0.0f);
	setBrushColour(ctx, COLOUR_PAL_DARKGREY);
	drawString(ctx, "Shearing H", (VWIDTH>>1), (VHEIGHT>>1)-100);
	
	setFont(ctx, &futural);
	setRenderFilter(ctx, RENDEROP_ROTATE_GLYPHS);
	setBrushColour(ctx, COLOUR_PAL_BLUE);
	drawString(ctx, "Hello World!", (VWIDTH>>1)-150, (VHEIGHT>>1)+100);

	setBrushColour(ctx, COLOUR_PAL_DARKGREY);
	setRenderFilter(ctx, RENDEROP_SHEAR_Y);
	setShearAngle(ctx, 0.0f, angle);
	drawString(ctx, "Shearing V", (VWIDTH>>1)-230, (VHEIGHT>>1));

	setBrushColour(ctx, COLOUR_PAL_HOMER);
	setRenderFilter(ctx, RENDEROP_NONE);
	setGlyphScale(ctx, 2.0f);
	snprintf(tbuffer, sizeof(tbuffer), "%.0f", angle);
	drawString(ctx, tbuffer, (VWIDTH>>1)+110, (VHEIGHT>>1)+35);
		
	setFont(ctx, &romand);
	setBrushColour(ctx, COLOUR_PAL_RED);
	setAspect(ctx, 1.0f, -1.0f);
	setRenderFilter(ctx, RENDEROP_NONE);
	setGlyphScale(ctx, 2.0f);
	snprintf(tbuffer, sizeof(tbuffer), "%.0f", angle);
	drawString(ctx, tbuffer, (VWIDTH>>1)+110, (VHEIGHT>>1)+80);
	setAspect(ctx, 1.0f, 1.0f);
	
	x = 60;
	y = (VHEIGHT>>1)+10;
	setFont(ctx, &romans);
	setBrushColour(ctx, COLOUR_PAL_MAGENTA);
	setGlyphScale(ctx, 1.0f);
	setAspect(ctx, 1.0f, angle/45.0f);
	drawString(ctx, "Aspect", x, y);

	box_t box;
	getStringMetrics(ctx, "Aspect", &box);
	drawRectangle(x+box.x1, y+box.y1, x+box.x2, y+box.y2, COLOUR_PAL_GREEN);
	setAspect(ctx, 1.0f, 1.0f);


	setFont(ctx, &romans);
	setBrushColour(ctx, COLOUR_PAL_BLUE_SEA_TINT);
	setBrush(ctx, BRUSH_DISK);
	setBrushStep(ctx, 1.0f);
	setGlyphScale(ctx, angle/40.5f);
	setBrushSize(ctx, angle/25.5f);
	
	getStringMetrics(ctx, "Scale", &box);
	drawString(ctx, "Scale", (VWIDTH-box.x2)/2.0f, (VHEIGHT-(box.y2+0.5f))-5.0f);
	
#endif




#if 0
	setFont(ctx, &futural);
	setRenderFilter(ctx, RENDEROP_NONE);
	setBrushColour(ctx, COLOUR_PAL_RED);
	setBrushStep(ctx, 1.0f);
	setAspect(ctx, 1.0f, 1.0f);
	setGlyphScale(ctx, 3.0f);

	setBrushSize(ctx, 2.0f);
	setBrush(ctx, BRUSH_STROKE_7);
	drawString(ctx, "Scale", 50, 250);
#endif

#if 0
		float x1 = 50.0f;
		float y1 = 50.0f;
		float x2 = 600.0f;
		float y2 = 400.0f;

		// cross product
		float x = (y1 * 1.0f) - (1.0f * y2);
		float y = (1.0f * x2) - (x1 * 1.0f);
		float length = sqrtf((x * x) + (y * y));
		//float r = fabsf(x/y);
		
		// set thickness;
		x *= 50.0f * (1.0f/length);
		y *= 50.0f * (1.0f/length);

		float xh = (x / 2.0f);	// center path by precomputing and using half thickness per side
		float yh = (y / 2.0f);

		drawTriangleFilled(x1+xh, y1+yh, x1-xh, y1-yh, x2+xh, y2+yh, COLOUR_PAL_RED);
		drawTriangleFilled(x1-xh, y1-yh, x2+xh, y2+yh, x2-xh, y2-yh, COLOUR_PAL_BLUE);
		
		drawLine(x1, y1, x2, y2, COLOUR_PAL_BLACK);
		drawLine(x1, y1, x1-x, y1-y, COLOUR_PAL_DARKGREEN);	// start
		drawLine(x2, y2, x2-x, y2-y, COLOUR_PAL_BLUE);		// end
#endif


#if 0
	// measure time-to-render only as time-to-display is not the benchmark we are looking for
	static uint32_t t3;

	uint32_t t = micros() - t3;
	t3 = micros();

	setRenderFilter(ctx, RENDEROP_NONE);
	setFont(ctx, &futural);
	setBrush(ctx, BRUSH_DISK);
	setBrushStep(ctx, 1.0f);
	setBrushSize(ctx, 1.0f);
	setAspect(ctx, 1.0f, 1.0f);
	setBrushColour(ctx, COLOUR_PAL_BLACK);
	setGlyphScale(ctx, 0.7f);
	
	//snprintf(tbuffer, sizeof(tbuffer), "%fs", (double)(t2-t3)/1000000.0);
	snprintf(tbuffer, sizeof(tbuffer), "%fs  %.1f", (double)(t)/1000.0, 1.0/((double)(t)/1000.0));
	drawString(ctx, tbuffer, 5, 10);
#endif

}
	

int main (int argc, char **args)
{
	if (!initDemoConfig("config.cfg"))
		return 0;

	lSetBackgroundColour(hw, lGetRGBMask(frame, LMASK_BLACK));
	lSetForegroundColour(hw, lGetRGBMask(frame, LMASK_WHITE));
	lClearFrame(frame);
	
	vfont_t context;
	setup(&context);
	
	const int num = 1000;
	const uint32_t t1 = GetTickCount();
	
	for (int i = 0; i < num; i++){
		doTests(&context);
		updateDisplay();
		//lSleep(20);
	}
	const uint32_t t2 = GetTickCount();
	printf("time: %i %f\n", (t2 - t1), (t2 - t1)/(float)num);

	updateDisplay();
	lSleep(5000);
	
	demoCleanup();	
	return 1;
}


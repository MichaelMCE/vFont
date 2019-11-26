

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
#include <math.h>

#include "vfont.h"
#include "hfont.h"






uint8_t renderBuffer[VWIDTH * VHEIGHT];


/*
static inline void drawPixel8 (const int x, const int y, const uint8_t colourIdx)
{
	uint8_t *pixels = (uint8_t*)renderBuffer;	
	pixels[(y*VWIDTH)+x] = colourIdx;
}
*/
static inline int getPixel1 (const uint8_t *pixels, const int pitch, const int x, const int y)
{
	return *(pixels+((y * pitch)+(x>>3))) >>(x&7)&0x01;
}

static inline void clipRect (int *x1, int *y1, int *x2, int *y2)
{
	if (*x1 < 0)
		*x1 = 0;
	else if (*x1 >= VWIDTH)
		*x1 = VWIDTH-1;
	if (x2 < 0)
		*x2 = 0;
	else if (*x2 >= VWIDTH)
		*x2 = VWIDTH-1;
	if (*y1 < 0)
		*y1 = 0;
	else if (*y1 >= VHEIGHT)
		*y1 = VHEIGHT-1;		
	if (*y2 < 0)
		*y2 = 0;
	else if (*y2 >= VHEIGHT)
		*y2 = VHEIGHT-1;
}

void drawBitmap (image_t *img, int x, int y, const uint16_t colour)
{
	const int srcPitch = CALC_PITCH_1(img->width);
		
	// center image
	x -= img->width>>1;
	y -= img->height>>1;
	int x2 = x+img->width;
	int y2 = y+img->height;
	
	clipRect(&x, &y, &x2, &y2);	
	x2 -= x; y2 -= y;
	
	for (int y1 = 0; y1 < y2; y1++, y++){
		int _x = x;
		for (int x1 = 0; x1 < x2; x1++, _x++){
			if (getPixel1(img->pixels, srcPitch, x1, y1))
				drawPixel(_x, y, colour);
		}
	}
}

static inline void swapf (float *a, float *b) 
{ 
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline void swap32 (int *a, int *b) 
{ 
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static inline void drawHLine (const int y, int x1, int x2, const uint16_t colour)
{
	if (x2 < x1) swap32(&x1, &x2);
	if (x1 < 0) x1 = 0;
	if (x2 >= VWIDTH) x2 = VWIDTH - 1;

	for (int x = x1; x <= x2; x++)
		drawPixel(x, y, colour);
}

#define REGION_TOP		0x1
#define REGION_BOTTOM	0x2
#define REGION_RIGHT	0x4
#define REGION_LEFT		0x8


static inline int findRegion (const int x, const int y)
{
	int code = 0;
	
	if (y >= VHEIGHT)
		code = REGION_TOP;			// top
	else if (y < 0)
		code |= REGION_BOTTOM;		// bottom

	if (x >= VWIDTH)
		code |= REGION_RIGHT;		// right
	else if ( x < 0)
		code |= REGION_LEFT;		// left

	return code;
}

#if 0
// slighty faster Cohen-Sutherland
static inline int clipLine (int x1, int y1, int x2, int y2, int *x3, int *y3, int *x4, int *y4)
{
    while (1){
        int code1 = findRegion(x1, y1);
        int code2 = findRegion(x2, y2);
        
        if (!(code1 | code2)){
        	*x3 = x1;
        	*x4 = x2;
        	*y3 = y1;
        	*y4 = y2;
            return 1;
        }
        
        if (code1&code2) //on same side of a region, hence outside
            return 0;
        
        if (!code1){ //pt1 inside rectangle, swap with pt 2
            swap32(&x1, &x2);
            swap32(&y1, &y2);
            code1 = code2;
        }
        
        //find intersection pts.
        //only do 1 intersection pt inside 1 iteration
        if (code1&REGION_TOP){
            y1 = y1+ ((y2-y1)*(-x1))/(x2-x1);
            x1 = 0;
            
        }else if (code1&REGION_RIGHT){
            y1 = y1+ ((y2-y1)*((VWIDTH-1)-x1))/(x2-x1);
            x1 = VWIDTH-2;
            
        }else if (code1&REGION_LEFT){
            x1 = x1 + ((x2-x1)*((VHEIGHT-1)-y1))/(y2 - y1);
            y1 = VHEIGHT-2;
            
        }else if (code1&REGION_BOTTOM){
            x1 = x1 + ((x2-x1)*(-y1))/(y2 - y1);
            y1 = 0;
        }
    }
    
    return 0;
}

#elif 0

// Liang barsky clip (floating point)
static inline int clipLine (const float x1, const float y1, const float x2, const float y2, int *x3, int *y3, int *x4, int *y4)
{
    const float delX = x2 - x1;
    const float delY = y2 - y1;
    
    const float p[] = {-delX,			//L
                	 delX,				//R
					 -delY,				//B
					 delY				//T
					};
    const float q[] = {x1 ,
					 VWIDTH-1 - x1,
					 y1,
					 VHEIGHT-1 - y1
					};
    float u1 = 0.0f;
    float u2 = 1.0f;
    
    for (int i = 0; i < 4; i++){
        if (!p[i]){								// || to boundary
            if (q[i] < 0.0f) return 0.0f;		// rejected
            continue;							// no intersection with current edge since parallel
        }
        
        float u = q[i] / p[i];
        
        if (p[i] < 0.0f)						// outside to inside
            u1 = max(u1, u);
        else if (p[i] > 0.0f)					// inside to outside
            u2 = min(u2, u);
        
    }
    
    if (u1 > u2) return 0;						// rejected

    *x3 = x1 + delX * u1;
    *y3 = y1 + delY * u1;
    *x4 = x1 + delX * u2;
    *y4 = y1 + delY * u2;

	return 1;
}

#elif 1
// clip using Cohen-Sutherland algorithm
static inline int clipLine (int x1, int y1, int x2, int y2, int *x3, int *y3, int *x4, int *y4)
{
  
	int accept = 0, done = 0;
	int code1 = findRegion(x1, y1); //the region outcodes for the endpoints
	int code2 = findRegion(x2, y2);
  
	const int h = VHEIGHT-1;
	const int w = VWIDTH-1;
  
	do{
		if (!(code1 | code2)){
			accept = /*done =*/ 1;  //accept because both endpoints are in screen or on the border, trivial accept
			break;
			
		}else if (code1&code2){
			//done = 1; //the line isn't visible on screen, trivial reject
			break;
    		
		}else{  //if no trivial reject or accept, continue the loop
			int x, y;
			int codeout = code1 ? code1 : code2;
			if (codeout&REGION_TOP){			//top
				x = x1 + (x2 - x1) * (h - y1) / (y2 - y1);
				y = h - 1;
			}else if (codeout&REGION_BOTTOM){	//bottom
				x = x1 + (x2 - x1) * -y1 / (y2 - y1);
				y = 0;
			}else if (codeout&REGION_RIGHT){	//right
				y = y1 + (y2 - y1) * (w - x1) / (x2 - x1);
				x = w - 1;
			}else{					//left
				y = y1 + (y2 - y1) * -x1 / (x2 - x1);
				x = 0;
			}
      
			if (codeout == code1){ //first endpoint was clipped
				x1 = x; y1 = y;
				code1 = findRegion(x1, y1);
			}else{ //second endpoint was clipped
				x2 = x; y2 = y;
        		code2 = findRegion(x2, y2);
			}
		}
	}while(!done);

	if (accept){
		*x3 = x1;
		*x4 = x2;
		*y3 = y1;
		*y4 = y2;
		return 1;
	}else{
		return 0;
	}
}
#endif

#ifdef LINE_FAST
static inline void drawLineFast (int x, int y, int x2, int y2, const int colour)
{
	if (!clipLine(x, y, x2, y2, &x, &y, &x2, &y2))
		return;

   	int yLonger = 0;
	int shortLen = y2 - y;
	int longLen = x2 - x;
	
	if (abs(shortLen) > abs(longLen)){
		swap32(&shortLen, &longLen);
		yLonger = 1;
	}
	int decInc;
	
	if (!longLen)
		decInc = 0;
	else
		decInc = (shortLen<<16) / longLen;


	if (yLonger) {
		if (longLen > 0) {
			longLen += y;
			
			for (int j = 0x8000+(x<<16); y <= longLen; ++y){
				drawPixel(j>>16, y, colour);
				j+=decInc;
			}
			return;
		}
		longLen += y;
		
		for (int j = 0x8000+(x<<16); y >= longLen; --y){
			drawPixel(j>>16, y, colour);
			j-=decInc;
		}
		return;
	}

	if (longLen > 0) {
		longLen += x;
		
		for (int j = 0x8000+(y<<16); x <= longLen; ++x){
			drawPixel(x, j>>16, colour);
			j+=decInc;
		}
		return;
	}
	
	longLen += x;
	
	for (int j = 0x8000+(y<<16); x >= longLen; --x){
		drawPixel(x, j>>16, colour);
		j-=decInc;
	}

}
#endif

#ifdef LINE_STD
static inline void drawLineStd (int x1, int y1, int x2, int y2, const uint16_t colour)
{
	if (!clipLine(x1, y1, x2, y2, &x1, &y1, &x2, &y2))
		return;

	
    const int dx = x2 - x1;
    const int dy = y2 - y1;

    if (dx || dy){
        if (abs(dx) >= abs(dy)){
            float y = y1 + 0.5f;
            float dly = dy / (float)dx;
            
            if (dx > 0){
                for (int xx = x1; xx<=x2; xx++){
                    drawPixel(xx, (int)y, colour);
                    y += dly;
                }
            }else{
                for (int xx = x1; xx>=x2; xx--){
                    drawPixel(xx, (int)y, colour);
                    y -= dly;
                }
			}
        }else{
           	float x = x1 + 0.5f;
           	float dlx = dx/(float)dy;

            if (dy > 0){
   	            for (int yy = y1; yy<=y2; yy++){
       	            drawPixel((int)x, yy, colour);
           	        x += dlx;
               	}
			}else{
                for (int yy = y1; yy >= y2; yy--){
   	                drawPixel((int)x, yy, colour);
       	            x -= dlx;
           	    }
			}
        }
    }else if (!(dx&dy)){
    	drawPixel(x1, y1, colour);
    }

}
#endif

#ifdef LINE_FASTEST8
static inline void drawLine8 (int x0, int y0, int x1, int y1, const uint8_t colour)
{
	if (!clipLine(x0, y0, x1, y1, &x0, &y0, &x1, &y1))
		return;
		
	int stepx, stepy;
	
	int dy = y1 - y0;
	if (dy < 0){
		dy = -dy;
		stepy = -VWIDTH;
	}else{
		stepy = VWIDTH;
	}
	dy <<= 1;
		
	int dx = x1 - x0;
	if (dx < 0){
		dx = -dx;
		stepx = -1;
	}else{
		stepx = 1;
	}
	dx <<= 1;
	
	y0 *= VWIDTH;
	y1 *= VWIDTH;

	uint8_t *pixels = (uint8_t*)renderBuffer;
	pixels[x0+y0] = colour;
	
	if (dx > dy){
	    int fraction = dy - (dx >> 1);
	    
	    while (x0 != x1){
	        if (fraction >= 0){
	            y0 += stepy;
	            fraction -= dx;
	        }
	        x0 += stepx;
	        fraction += dy;
	        pixels[x0+y0] = colour;
	    }
	}else{
	    int fraction = dx - (dy >> 1);
	    
	    while (y0 != y1){
	        if (fraction >= 0){
	            x0 += stepx;
	            fraction -= dy;
	        }
	        y0 += stepy;
	        fraction += dx;
	        pixels[x0+y0] = colour;
	    }
	}
	return;
}
#endif

#ifdef LINE_FASTEST16
static inline void drawLine16 (int x0, int y0, int x1, int y1, const uint16_t colour)
{
	if (!clipLine(x0, y0, x1, y1, &x0, &y0, &x1, &y1))
		return;
		
	int stepx, stepy;
	
	int dy = y1 - y0;
	if (dy < 0){
		dy = -dy;
		stepy = -VWIDTH;
	}else{
		stepy = VWIDTH;
	}
	dy <<= 1;
		
	int dx = x1 - x0;
	if (dx < 0){
		dx = -dx;
		stepx = -1;
	}else{
		stepx = 1;
	}
	dx <<= 1;
	
	y0 *= VWIDTH;
	y1 *= VWIDTH;

	uint16_t *pixels = (uint16_t*)renderBuffer;
	pixels[x0+y0] = colour;
	
	if (dx > dy){
	    int fraction = dy - (dx >> 1);
	    
	    while (x0 != x1){
	        if (fraction >= 0){
	            y0 += stepy;
	            fraction -= dx;
	        }
	        x0 += stepx;
	        fraction += dy;
	        pixels[x0+y0] = colour;
	    }
	}else{
	    int fraction = dx - (dy >> 1);
	    
	    while (y0 != y1){
	        if (fraction >= 0){
	            x0 += stepx;
	            fraction -= dy;
	        }
	        y0 += stepy;
	        fraction += dx;
	        pixels[x0+y0] = colour;
	    }
	}
	return;
}
#endif

void drawLine (const int x1, const int y1, const int x2, const int y2, const uint16_t colour)
{
#if LINE_STD
	// slowest: standard Bresenham algorithm
	drawLineStd(x1, y1, x2, y2, colour);		// 94.0
	
#elif LINE_FAST	
	// fast: similar to Bresenham but forgoes accuracy for performance
	drawLineFast(x1, y1, x2, y2, colour);		// 58.7

#elif LINE_FASTEST16
	// faster line rountine - draw direct to 16bit buffer
	drawLine16(x1, y1, x2, y2, colour);			// 27.9

#elif LINE_FASTEST8
	// fastest line rountine - draw direct to 8bit buffer
	drawLine8(x1, y1, x2, y2, colour);
#endif

}

static inline void drawVLine (const int x, const int y, const int h, const uint16_t colour)
{
	drawLine(x, y, x, y+h-1, colour);
}

void drawCircleFilled (const int x0, const int y0, const float radius, const uint16_t colour)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2.0f * radius;
	int x = 0;
	int y = radius;
	int ylm = x0 - radius;


	while (x < y){
		if (f >= 0){
			drawVLine(x0 + y, y0 - x, 2 * x + 1, colour);
			drawVLine(x0 - y, y0 - x, 2 * x + 1, colour);
			
			ylm = x0 - y;
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		
		x++;
		ddF_x += 2;
		f += ddF_x;

		if ((x0 - x) > ylm){
			drawVLine(x0 + x, y0 - y, 2 * y + 1, colour);
			drawVLine(x0 - x, y0 - y, 2 * y + 1, colour);
		}
	}
	drawVLine(x0, y0 - radius, 2.0f * radius + 1.0f, colour);
}

static inline void drawCirclePts (const int xc, const int yc, const int x, const int y, const uint16_t colour)
{
	drawPixel(xc+y, yc-x, colour);
	drawPixel(xc-y, yc-x, colour);
	drawPixel(xc+y, yc+x, colour);
	drawPixel(xc-y, yc+x, colour);
	drawPixel(xc+x, yc+y, colour);
	drawPixel(xc-x, yc+y, colour);
	drawPixel(xc+x, yc-y, colour);
	drawPixel(xc-x, yc-y, colour);
}

void drawCircle (const int xc, const int yc, const int radius, const uint16_t colour)
{
	float x = 0.0f;
	float y = radius;
	float p = 1.25f - radius;

	drawCirclePts(xc, yc, x, y, colour);
	
	while (x < y){
		x += 1.0f;
		if (p < 0.0f){
			p += 2.0f*x+1.0f;
		}else{
			y -= 1.0f;
			p += 2.0f*x+1.0f-2.0f*y;
		}
		drawCirclePts(xc, yc, x, y, colour);
	}
}

void drawRectangleFilled (int x1, int y1, int x2, int y2, const uint16_t colour)
{
	clipRect(&x1, &y1, &x2, &y2);
	
	for (int y = y1; y <= y2; y++)
		drawHLine(y, x1, x2, colour);
}

void drawRectangle (int x1, int y1, int x2, int y2, const uint16_t colour)
{
	clipRect(&x1, &y1, &x2, &y2);
	
	drawLine(x1, y1, x2, y1, colour);		// top
	drawLine(x1, y2, x2, y2, colour);		// bottom
	drawLine(x1, y1+1, x1, y2-1, colour);	// left
	drawLine(x2, y1+1, x2, y2-1, colour);	// right
}

void drawTriangle (const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const uint16_t colour)
{
	drawLine(x1, y1, x2, y2, colour);
	drawLine(x2, y2, x3, y3, colour);
	drawLine(x1, y1, x3, y3, colour);
}

void drawTriangleFilled (const int x0, const int y0, const int x1, const int y1, const int x2, const int y2, const uint16_t colour)
{
	float XA = 0.0f, XB = 0.0f;
  	float XA1 = 0.0f, XB1 = 0.0f, XC1 = 0.0f;
  	float XA2 = 0.0f, XB2 = 0.0f;
  	float XAd, XBd; 
  	float HALF = 0.0f;
  	
	int t = y0;
	int b = y0;
	int CAS = 0;
	
	if (y1 < t){
		t = y1;
		CAS = 1;
	}
	if (y1 > b)
		b = y1;
		
	if (y2 < t){
		t = y2;
		CAS = 2;
	}
	if (y2 > b)
		b = y2;
   	
	if (CAS == 0){
		XA = x0;
		XB = x0;
		XA1 = (x1-x0)/(float)(y1-y0);
		XB1 = (x2-x0)/(float)(y2-y0);
		XC1 = (x2-x1)/(float)(y2-y1);
		
		if (y1<y2){
			HALF = y1;
      		XA2 = XC1;
      		XB2 = XB1;
    	}else{
    		HALF = y2;
      		XA2 = XA1;
      		XB2 = XC1;
    	}
		if (y0 == y1)
			XA = x1;
		if (y0 == y2)
			XB = x2;
  	}else if (CAS == 1){
    	XA = x1;
    	XB = x1;
    	XA1 = (x2-x1)/(float)(y2-y1);
    	XB1 = (x0-x1)/(float)(y0-y1);
    	XC1 = (x0-x2)/(float)(y0-y2);
    	
    	if ( y2 < y0){
    		HALF = y2;
      		XA2 = XC1;
      		XB2 = XB1;
    	}else{
    		HALF = y0;
      		XA2 = XA1;
      		XB2 = XC1;
    	} 
    	if (y1 == y2)
    		XA = x2;
    	if (y1 == y0)
			XB = x0;
	}else if (CAS == 2){
		XA = x2;
		XB = x2;
    	XA1 = (x0-x2)/(float)(y0-y2);
    	XB1 = (x1-x2)/(float)(y1-y2);
    	XC1 = (x1-x0)/(float)(y1-y0);
    	if (y0<y1){
    		HALF = y0;
      		XA2 = XC1;
      		XB2 = XB1;
    	}else{
    		HALF = y1;
      		XA2 = XA1;
      		XB2 = XC1;
    	}
    	if (y2 == y0)
    		XA = x0;
    	if (y2 == y1)
    		XB = x1;
	}
  
	if (XA1 > XB1){
		swapf(&XA, &XB);
		swapf(&XA1, &XB1);
		swapf(&XA2, &XB2);
	}
  
	for (int y = t; y < HALF; y++){
		XAd = XA;
		XBd = XB;
		
		for (int x = XAd; x <= XBd; x++)
			drawPixel(x, y, colour);
		XA += XA1;
		XB += XB1;	
	}

	for (int y = HALF; y <= b; y++){
		XAd = XA;
		XBd = XB;
		
		for (int x = XAd; x <= XBd; x++)
			drawPixel(x, y, colour);
		XA += XA2;
		XB += XB2;	
	}
}


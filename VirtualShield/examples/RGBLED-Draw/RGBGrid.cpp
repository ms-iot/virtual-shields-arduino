
#include "RGBGrid.h"

#define _TCHAR String

extern "C" {
#include <string.h>
#include <stdlib.h>
}

RGBGrid::RGBGrid(int width, int height) : RGBStrip()
{
	this->width = width;
	this->height = height;
	this->count = width*height;
}

void RGBGrid::setPixel(int x, int y, ARGB color)
{
	int pixel = XYtoPixel(x,y);
	RGBStrip::SetPixel(pixel, (BYTE) color.red, (BYTE) color.green, (BYTE) color.blue);
}

int RGBGrid::XYtoPixel(int x, int y) {
    int ix = width-1-x;
	int iy = height-1-y;
	int pixel = ((int)(iy/2))*(width*2) + (iy%2 == 0 ? ix : 15-ix);
	return pixel;
}
/*
int downLevel(int drop)
{
	int div16 = drop / 16;
	int mod16 = drop % 16;
	return ((div16 + (mod16 > 7)) * 16 - 1) - mod16;
}
*/
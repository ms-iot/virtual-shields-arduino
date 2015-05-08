
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
	int pixel = (count-1) - (y*(height + (x%2==1))) - x;
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
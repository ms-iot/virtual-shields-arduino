/*
    Copyright(c) Microsoft Open Technologies, Inc. All rights reserved.

    The MIT License(MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "RGBStrip.h"

#define _TCHAR String

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const int stripClock = 2;
const int stripData = 3;
const int stripLen = 48;

BYTE r0, b0, g0, r, g, b;

// Array of stored Pixel values
PIXEL_VALUES Pixels[stripLen];

void RGBStrip::begin()
{
    // Set pins to outputs
    pinMode(stripClock, OUTPUT);
    pinMode(stripData, OUTPUT);
    digitalWrite(stripClock, LOW);
    digitalWrite(stripData, LOW);
	clear();
}

void RGBStrip::clear()
{
	// Reset all the pixels
	for (int i = 0; i < stripLen; i++)
	{
		SetPixel(i, 0, 0, 0);
	}

	ShiftAllPixels();
}

_PIXEL_VALUES RGBStrip::getPixel(int pos)
{
	return Pixels[pos];
}

void RGBStrip::setDim(int dim) 
{
	this->dim = dim == 0 ? 1 : dim;
}

void RGBStrip::setSpeed(int speed) 
{
	this->speed = speed;
}

int sgn(int value) {
	return value == 0 ? 0 : (value < 1 ? -1 : 1);
}

void RGBStrip::setAll(BYTE r, BYTE g, BYTE b) {
  int i;
  r0 = r/dim;
  b0=b/dim;
  g0=g/dim;

  tick();
}

void RGBStrip::tick() {
	r = speed == 0 || abs(r0-r) < speed ? r0 : r+sgn(r0-r)*speed;
	g = speed == 0 || abs(g0-g) < speed ? g0 : g+sgn(g0-g)*speed;
	b = speed == 0 || abs(b0-b) < speed ? b0 : b+sgn(b0-b)*speed;
	
	for (int i = 0; i < stripLen; i++)
    {
        SetPixel(i, r, g, b);
    }
	
    ShiftAllPixels();
}

void RGBStrip::SetPixels(int set[], int len, BYTE r, BYTE g, BYTE b)
{
	for (int i = 0; i < len; i++)
	{
		SetPixel(set[i], r, g, b);
	}
}

// Sets the pixel color in our array
void RGBStrip::SetPixel(int pixel, BYTE Red, BYTE Green, BYTE Blue)
{
    if (pixel < stripLen)
    {
        Pixels[pixel].Red = Red | 0x80;
        Pixels[pixel].Green = Green | 0x80;
        Pixels[pixel].Blue = Blue | 0x80;
    }
}

void RGBStrip::SetPixel(int pixel, _PIXEL_VALUES values)
{
	if (pixel < stripLen)
	{
		Pixels[pixel].Red = values.Red;
		Pixels[pixel].Green = values.Green;
		Pixels[pixel].Blue = values.Blue;
	}
}

// Sends the color of a pixel to the strip
void RGBStrip::ShiftPixel(int pixel)
{
    PPIXEL_VALUES PixelValues = &Pixels[pixel];
    BYTE bit;
    int i;

    for (i = 7; i >= 0; i--)
    {
        bit = (PixelValues->Green >> i) & 0x01;
        digitalWrite(stripData, bit);
        digitalWrite(stripClock, HIGH);
        digitalWrite(stripClock, LOW);
    }
    for (i = 7; i >= 0; i--)
    {
        bit = (PixelValues->Red >> i) & 0x01;
        digitalWrite(stripData, bit);
        digitalWrite(stripClock, HIGH);
        digitalWrite(stripClock, LOW);
    }
    for (i = 7; i >= 0; i--)
    {
        bit = (PixelValues->Blue >> i) & 0x01;
        digitalWrite(stripData, bit);
        digitalWrite(stripClock, HIGH);
        digitalWrite(stripClock, LOW);
    }
}

// Sends all the pixel colors to the strip
void RGBStrip::ShiftAllPixels()
{
    int i;

    digitalWrite(stripData, 0);
    for (i = 0; i < 8; i++)
    {
        digitalWrite(stripClock, HIGH);
        digitalWrite(stripClock, LOW);
    }
    for (i = 0; i < 8; i++)
    {
        digitalWrite(stripClock, HIGH);
        digitalWrite(stripClock, LOW);
    }
    for (i = 0; i < 8; i++)
    {
        digitalWrite(stripClock, HIGH);
        digitalWrite(stripClock, LOW);
    }

    for (i = 0; i < stripLen; i++)
    {
        ShiftPixel(i);
    }

    digitalWrite(stripData, 0);
    for (i = 0; i < 8; i++)
    {
        digitalWrite(stripClock, HIGH);
        digitalWrite(stripClock, LOW);
    }
    for (i = 0; i < 8; i++)
    {
        digitalWrite(stripClock, HIGH);
        digitalWrite(stripClock, LOW);
    }
}
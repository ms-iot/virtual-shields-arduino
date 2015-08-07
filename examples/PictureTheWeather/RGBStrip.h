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

#ifndef RGBStrip_h
#define RGBStrip_h

#define BYTE byte

#include "Arduino.h"

typedef struct _PIXEL_VALUES {
	BYTE Green;
	BYTE Red;
	BYTE Blue;
} PIXEL_VALUES, *PPIXEL_VALUES;

class RGBStrip {
	public:
		void begin();
		void setAll(BYTE r, BYTE g, BYTE b);
		void setDim(int dim);
		void setSpeed(int speed);
		void tick();
		void SetPixel(int pixel, BYTE Red, BYTE Green, BYTE Blue);
		void SetPixels(int set[], int len, BYTE r, BYTE g, BYTE b);
		void ShiftAllPixels();
		_PIXEL_VALUES getPixel(int pos);
		void SetPixel(int pixel, _PIXEL_VALUES values);
		void clear();
private:
		int speed = 0;
		int dim = 1;
	
		void ShiftPixel(int pixel);
};

#endif RGBStrip_h

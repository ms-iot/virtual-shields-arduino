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

#ifndef RGBStripMatrix_h
#define RGBStripMatrix_h

#include "Arduino.h"
#include "SensorModels.h"
#include <SPI.h>

class RGBStripMatrix {
	public:
		void begin();
		void clear();
		void makeDot(ARGB dotColor, float accelX, float accelY);
	private:
		int stripCount = 48;                      // Ideally should be an even number
		int stripLength = maxY = 48;              // Ideally should be an even number
		int numLEDs = stripCount * stripLength;
		int magicPixel = 947;                     // Weirdly broken pixel on the matrix.  Have to skip or alignment breaks.

                int maxX = stripCount;
                int maxY = stripLength;
		int xMidPoint = maxX/2;
		int yMidPoint = maxY/2;

		void sendResetBits();
		void pushPixel(ARGB argb);
		void pushPixel(uint8_t Red, uint8_t Green, uint8_t Blue);
		void pushWhite();
		void pushBlack();
		void setAll(uint8_t Red, uint8_t Green, uint8_t Blue);
};

#endif RGBStripMatrix_h

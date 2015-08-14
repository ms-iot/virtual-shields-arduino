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

#include <SPI.h>
#include "RGBStripMatrix.h"

void RGBStripMatrix::begin()
{
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}

// Clamps the value to within a max/min
int clamp (int source, int minValue, int maxValue)
{
  return source>minValue?(source<maxValue?source:maxValue):minValue;
}

// Takes a 0-1 ranged float value for X and Y, and makes a dot at that relative position
void RGBStripMatrix::makeDot(ARGB dotColor, float accelX, float accelY)
{
  // Figure out which X/Y LED the acceleration values map to
  int ledX = (int)(accelX * xMidPoint) + xMidPoint;
  int ledY = (int)(accelY * yMidPoint) + yMidPoint;
  
  // Make sure we didn't somehow get outside the valid range
  ledX = clamp(ledX, 0, maxX);
  ledY = clamp(ledY, 0, maxY);
  
  sendResetBits();
  
  int currentPixel = 0;
  
  for (int x = 0; x < maxX; x++)
  {
    // LED strip is wired so that even columns start with pixel 0 at the bottom,
    // and odd columns start with pixel 0 at the top...  So every other column we
    // need to flip which direction we're drawing from.
    boolean flip = (x % 2) == 0;
    for (int y = 0; y < maxY; y++)
    {
      currentPixel++;
      int adjustedY = y;
      if (flip)
      {
        adjustedY = maxY - 1 - y;
      }
      
      if(currentPixel == magicPixel)
      {
        continue;
      }
      
      // If the current pixel is within a few pixel radius of where we mapped 
      // the accelerometer values, draw the pixel with the specified color
      if ((x >= ledX - 1 & x <= ledX + 1) & (adjustedY >= ledY - 1 & adjustedY <= ledY + 1))
      {
        pushPixel(dotColor);
      }
      else
      {
        pushBlack();
      }
    }
  }
  
  sendResetBits();
}

// Sets all the pixels in the matrix to one color
void RGBStripMatrix::setAll(uint8_t Red, uint8_t Green, uint8_t Blue)
{
  sendResetBits();
  
  for (int stripNum = 0; stripNum < stripCount; stripNum++)
  {
      int stripStart = stripNum * stripLength;
      for (int pixelNum = 0; pixelNum < stripLength; pixelNum++)
      {
          pushPixel(Red, Green, Blue);
      }
  }
  
  sendResetBits();
}

// Sends bits to the matrix to let the LED controllers know the data is done/beginning
void RGBStripMatrix::sendResetBits()
{
  for(uint16_t i=((numLEDs+31)/32); i>0; i--) SPI.transfer(0);
} 

// Pushes an ARGB value to the current pixel
void RGBStripMatrix::pushPixel(ARGB color)
{
  SPI.transfer((color.green >> 2) | 0x80);
  SPI.transfer((color.red >> 2) | 0x80);
  SPI.transfer((color.blue >> 2) | 0x80);
}

// Pushes an already adjusted 128-255 ranged set of bytes
void RGBStripMatrix::pushPixel(uint8_t red, uint8_t green, uint8_t blue)
{
  SPI.transfer(green);
  SPI.transfer(red);
  SPI.transfer(blue);
}

// Pushes white to the current pixel
void RGBStripMatrix::pushWhite()
{
  pushPixel(0xFF, 0xFF, 0xFF);
}

// Pushes black to the current pixel
void RGBStripMatrix::pushBlack()
{
  pushPixel(0x80, 0x80, 0x80);
}

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

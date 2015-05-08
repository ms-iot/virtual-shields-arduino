#ifndef RGBGrid_h
#define RGBGrid_h

#define BYTE byte

#include "Arduino.h"
#include "RGBStrip.h"
#include "SensorModels.h"

class RGBGrid : public RGBStrip {
	public:
	    RGBGrid(int width, int height);
		void setPixel(int x, int y, ARGB color);
	private:
		int width;
		int height;
		int count;
		int XYtoPixel(int x, int y);

};

#endif RGBGrid_h

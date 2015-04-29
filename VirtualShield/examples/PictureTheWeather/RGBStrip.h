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

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

#ifndef Graphics_h
#define Graphics_h

#include "Text.h"
#include "Sensor.h"

const PROGMEM char HorizontalAlignment[] = "HorizontalAlignment";
const PROGMEM char Foreground[] = "Foreground";

enum Orientation
{
	Orientation_None = 0,
	Orientation_Landscape = 1,
	Orientation_Portrait = 2,
	Orientation_LandscapeFlipped = 4,
	Orientation_PortraitFlipped = 8
};

class Graphics : public Text
{
public:
    Graphics(const VirtualShield &shield);

	int drawAt(unsigned int x, unsigned int y, String text, String tag = (const char*)0, ARGB argb = static_cast<uint32_t>(0));
	
	int drawImage(unsigned int x, unsigned int y, String url, String tag = (const char*)0, unsigned int width = 0, unsigned int height = 0);

	int addButton(unsigned int x, unsigned int y, String text, String tag = (const char*) 0);

	int fillRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, 
        ARGB argb, String tag = (const char*)0, bool enableExtendedEvents = false);

    int change(unsigned int id, ARGB argb);

	int line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ARGB color, unsigned int weight = 1);
	int input(unsigned int x, unsigned int y, bool multiline = false, String text = (const char*) 0, unsigned int width = 0, unsigned int height = 0);

	int orientation(int autoRotationPreferences = -1);

	int enableTouch(bool enable);

	bool isButtonClicked(String tag, ShieldEvent* shieldEvent = 0);
    bool isButtonClicked(int id, ShieldEvent* shieldEvent = 0);
	bool isTouchEvent(ShieldEvent* shieldEvent = 0);
	bool isPressed(int id, ShieldEvent* shieldEvent = 0);
	bool isPressed(String tag, ShieldEvent* shieldEvent = 0);
	bool isReleased(int id, ShieldEvent* shieldEvent = 0);
	bool isReleased(String tag, ShieldEvent* shieldEvent = 0);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;

private:
	const char* area;
};

#endif
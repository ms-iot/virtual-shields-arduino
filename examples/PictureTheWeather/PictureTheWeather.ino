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

#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <Text.h>
#include <Web.h>
#include <Recognition.h>
#include <Geolocator.h>

#include "RGBStrip.h"

VirtualShield shield;
Text screen = Text(shield);
Geolocator gps = Geolocator(shield);
Web web = Web(shield);
Recognition speech = Recognition(shield);

RGBStrip strip;

int when = 0;
unsigned int weatherHashes[3];
unsigned int weatherHashCount;

const PROGMEM char RECOGNITIONTEXT[] =
"today,tomorrow,in 2 days,in 3 days,in 4 days,in 5 days,in 6 days,in 7 days,show thunderstorms,show rain,show mostly cloudy,show sunny,show clear at night,show partly cloudy,show showers,strike,lightning";

enum
{
	Sunny = 2049,
	Clear = 49059,

	Cloudy = 59736,

	Chance = 24982,
	Chc = 38710,
	Likely = 45926,
	Mostly = 14784,
	Partly = 1588,
	Heavy = 18937,
	Strong = 22737,
	Moderate = 22245,

	Showers = 2743,
	Rain = 25950,
	Thunderstorms = 10146,

	Tonight = 20685,
	Overnight = 56786,
	Strike = 26762,
	Lightning = 37184
};

static const unsigned int idToHash[8][2] = { { Thunderstorms, 0 }, { Rain, 0 }, { Mostly, Cloudy }, { Sunny, 0 }, { Clear, 0 }, { Partly, Cloudy }, { Showers, 0 }, {Strike, 0} };

bool isAny(const unsigned int weatherType, const unsigned int *hashes = weatherHashes, const unsigned int hashCount = weatherHashCount)
{
	for (int i = 0; i < hashCount; i++)
	{
		if ((unsigned int)hashes[i] == (unsigned int)weatherType)
		{
			return true;
		}
	}

	return false;
}

bool sunOrMoon = false;
bool cloudy = false;
bool rain = false;
bool lightning = false;
int intensity = 1;

bool night = false;
bool strike = false;

void setWeatherPicture(const unsigned int *hashes = weatherHashes, unsigned int hashCount = weatherHashCount)
{
	strike = isAny(Strike, hashes, hashCount) || isAny(Lightning, hashes, hashCount);
	if (strike)
	{
		return;
	}

	lightning = isAny(Thunderstorms, hashes, hashCount);
	rain = lightning || isAny(Showers, hashes, hashCount) || isAny(Rain, hashes, hashCount);
	bool mostly = isAny(Mostly, hashes, hashCount);
	bool partly = isAny(Partly, hashes, hashCount);
	cloudy = rain || mostly || partly || isAny(Cloudy, hashes, hashCount) ;
	sunOrMoon = mostly || partly || isAny(Sunny, hashes, hashCount) || isAny(Clear, hashes, hashCount);

	intensity = 1 + isAny(Rain, hashes, hashCount) + isAny(Thunderstorms, hashes, hashCount) * 2
		+ isAny(Moderate, hashes, hashCount) + (isAny(Heavy, hashes, hashCount) || isAny(Strong, hashes, hashCount)) * 2;

	if ((sunOrMoon && partly) || (!sunOrMoon && mostly))
	{
		intensity++; // if partly sunny or mostly cloudy - increase cloud intensity
	}

	strip.clear();

	//if (rain)
	//{
            //media.playAudio("rain.mp3");
        //}
}

void speechEvent(ShieldEvent* shieldEvent)
{
	int id = shieldEvent->resultId;
	if (id > 0 && id < 9)
	{
		screen.printAt(14, shieldEvent->result);
		when = (id - 1) * 2;
		gps.get();
	}
	else if (id > 8)
	{
		int index = id - 9;
		night = index == 4;
		setWeatherPicture(idToHash[index], 2);
	}

	speech.listenFor(EPtr(MESSAGE, RECOGNITIONTEXT), false, Confidence_Medium);
}

byte tick = 0;

int sun[] = {47, 46, 33, 32};
int clouds[] = {44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 31, 30, 29};

long nextCheck = 0;
int rainIndex = 0;
const int rainMax = 10;
const int rainShift = 3;
const int rainSpeed = 200;
const int rainTick = 127;
const int rainTock = 10;
int tock = 0;
int raindrops[rainMax];
int raintraces[rainMax];

int downLevel(int drop)
{
	int div16 = drop / 16;
	int mod16 = drop % 16;
	return ((div16 + (mod16 > 7)) * 16 - 1) - mod16;
}

void weatherTick()
{
	byte cycle = tick > 127 ? tick - 128 : 127 - tick;
	byte upcycle = max(cycle, 32);

	bool updated = false;

	if (sunOrMoon)
	{
		strip.SetPixels(sun, 4, upcycle * !night, upcycle * !night, upcycle * night);
		updated = true;
	}

	if (tick == 0 && (cloudy || lightning))
	{
		strip.SetPixels(clouds, 13, 0, 0, 0);
		strip.SetPixels(clouds, min(intensity * 4, 13), 0x20, 0x20, 0x20);
		updated = true;
	}

	if (tick % (rainTick / intensity) == 0)
	{
		tock++;
	}

	if (rain && (tick % (rainTick / intensity)) == 0 && tock % rainTock == 0)
	{
		for (int j = 0; j < rainMax; j++)
		{
			if (raintraces[j] > 0 && raintraces[j] < 30)
			{
				strip.SetPixel(raintraces[j], 0, 0, 0);
				raintraces[j] = 0;
			}

			if (raindrops[j] > 0 && raindrops[j] < 30)
			{
				strip.SetPixel(raindrops[j], 0, 0, 0x08);

				raintraces[j] = raindrops[j];
				raindrops[j] = max(0, downLevel(raindrops[j]));
				if (raindrops[j] > 0 && raindrops[j] < 30)
				{
					strip.SetPixel(raindrops[j], 0, 0, 0x4f);
				}
			}
		}

		for (int i = 0; i < intensity; i++)
		{
			for (int j = 0; j < rainMax; j++)
			{
				if (raindrops[j] < 1 || raindrops[j] > 29)
				{
					raindrops[j] = (int)random(24, 29);
					strip.SetPixel(raindrops[j], 0, 0, 0x4f);
					break;
				}
			}
		}

		updated = true;
	}

	if (strike || lightning && (tick % rainTick) == random(0, rainTick) && tock % rainTock == random(0, rainTock))
	{
		int flash = random(0, 40 / intensity);
		if (strike || flash < 2)
		{
			if (!strike)
			{
				int pixel = random(0, 13);
				strip.SetPixel(clouds[pixel], 0x7f, 0x7f, 0);
			}

			_PIXEL_VALUES lit[6];
			int pos[6];
			if (strike || random(0, 100 / intensity) < 2)
			{
				int count = 0;
				int pixel = random(40,48);
				while (pixel > 0)
				{
					lit[count] = strip.getPixel(pixel);
					pos[count++] = pixel;
					strip.SetPixel(pixel, 0x7f, 0x7f, 0);
					strip.ShiftAllPixels();
					pixel = downLevel(pixel) + random(-1, 2);
				}

				strip.ShiftAllPixels();
				delay(100);
				for (int i = 0; i < count; i++)
				{
					strip.SetPixel(pos[i], lit[i]);
					strip.ShiftAllPixels();
				}

				strike = false;
			}
		}

		updated = true;
	}

	if (updated)
	{
		strip.ShiftAllPixels();
	}

	++tick;
}

const int MAX_RESPONSE_LENGTH = 100;
char response[MAX_RESPONSE_LENGTH];

const PROGMEM char WEATHERMESSAGE[] = "The forecast for ~ for ~ is ~.";

void webEvent(ShieldEvent* shieldEvent)
{
	web.getResponse(response, 100);

	EPtr eptrs[4];
	int count = EPtr::parse(response, eptrs, 3, '|', 1);
	eptrs[0] = EPtr(0, WEATHERMESSAGE);

	screen.printAt(7, shieldEvent->resultId);
	screen.printAt(8, EPtr(Format, MESSAGE, eptrs, 3));

	unsigned int timeHash[1];
	int timeHashCount = shield.parseToHash(eptrs[2].value, timeHash, 1, ' ', eptrs[2].length);

	night = timeHash[0] == Tonight || timeHash[0] == Overnight;

	weatherHashCount = shield.parseToHash(eptrs[3].value, weatherHashes, 3, ' ');

	setWeatherPicture();
}

// in the JSON returned by weather.gov,
// location.areaDescription is the name of the place (city name or # miles directionally from another location).
// '&' means keep previous results. '^' means restart parsing from higher scope (whole document in this case)
// time.startPeriodName[~] is the time period (~) is replaced with a number (whenEptr below),							   
// data.weather[~] is the forecast, (~) is replaced with a number  (whenEptr below)		
// all three are returned as result1|result2|result3 into the webEvent() as a response
const PROGMEM char INSTRUCTIONS[] = "J:location.areaDescription|&^J:time.startPeriodName[~]|&^J:data.weather[~]";
const PROGMEM char WEATHERURL[] = "http://forecast.weather.gov/MapClick.php?lat=~&lon=~&FcstType=json";
const PROGMEM char GPSMESSAGE[] = "Your coordinates are ~ latitude and ~ longitude.";

void gpsEvent(ShieldEvent* shieldEvent)
{
	EPtr lat = EPtr(0, gps.Latitude);
	EPtr lon = EPtr(0, gps.Longitude);

	EPtr printItems[] = {EPtr(0, GPSMESSAGE), lat, lon};

	screen.printAt(3, EPtr(Format, MESSAGE, printItems, 3));

	EPtr urlItems[] = {EPtr(0, WEATHERURL), lat, lon};
	EPtr whenEptr = EPtr(0, when);
	EPtr instructions[] = {EPtr(0, INSTRUCTIONS), whenEptr, whenEptr};

	// issue get of the weather forecast parsed web page
	web.get(EPtr(Format, URL, urlItems, 3), EPtr(Format, PARSE, instructions, 3));
}

void refresh(ShieldEvent* shieldEvent)
{
	screen.clear();
	screen.printAt(1, "Web Weather Indicator");

	gps.get();
	speech.listenFor(EPtr(MESSAGE, RECOGNITIONTEXT), false, Confidence_Medium);
}

void setup()
{
	shield.setOnRefresh(refresh);
	gps.setOnEvent(gpsEvent);
	web.setOnEvent(webEvent);
	speech.setOnEvent(speechEvent);

	strip.begin();
	shield.begin();
}

const long ThirtyMinutes = 1000 * 60 * 30 * 1000;
const long OneMinute = 1000 * 60 * 1000;

void loop()
{
	weatherTick();
	shield.checkSensors();
}		 
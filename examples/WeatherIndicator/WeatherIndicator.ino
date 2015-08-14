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
#include <Geolocator.h>

VirtualShield shield;
Text screen = Text(shield);
Geolocator gps = Geolocator(shield);
Web web = Web(shield);

const int SUNNY_PIN = 8;
const int CLOUDY_PIN = 9;
const int RAINY_PIN = 10;

void setWeatherPicture(String condition) 
{
	digitalWrite(SUNNY_PIN, condition.endsWith("Clear") || condition.endsWith("Partly Cloudy") ? HIGH : LOW);
	digitalWrite(CLOUDY_PIN, condition.endsWith("Cloudy") ? HIGH : LOW);
	digitalWrite(RAINY_PIN, condition.endsWith("Rain") || condition.endsWith("Showers") ? HIGH : LOW);
}

const int MAX_RESPONSE_LENGTH = 120;
char response[MAX_RESPONSE_LENGTH];

bool checkWeather() 
{
	// blocking on getting a single GPS reading...
	if (gps.get()) {

		String youAreHere = String("Your coordinates are:" +
			String(gps.Latitude) + "," + String(gps.Longitude));

		screen.printAt(2, youAreHere);
		
		String url = String("http://forecast.weather.gov/MapClick.php?lat=" + 
			String(gps.Latitude) + "&lon=" + String(gps.Longitude) + "&FcstType=json");

		// in the JSON returned by weather.gov,
		// data.weather[0] is the forecast							   
		// '&' means keep previous results. '^' means restart parsing from higher scope (whole document in this case)
		// location.areaDescription is the name of the place (city name or # miles directionally from another location).
		// retrieves those two pieces and 'F'ormats them into a string.
		String parse = "J:data.weather[0]|&^J:location.areaDescription|F:The forecast for {1} is {0}";
		
		// blocking on a getting the weather forecast parsed web page
		if (web.get(url, parse)) {

			// get the text response into the 'response' buffer up to MAX_RESPONSE_LENGTH.
			// this should be something like "The forecast for Redmond WA is Clear"
			web.getResponse(response, MAX_RESPONSE_LENGTH);

			screen.printAt(5, response);

			setWeatherPicture(response);

			return true;
		}
	}

	return false;
}

void setup()
{
	shield.begin();
	screen.clear();

	pinMode(SUNNY_PIN, OUTPUT);
	pinMode(CLOUDY_PIN, OUTPUT);
	pinMode(RAINY_PIN, OUTPUT);

	screen.printAt(0, "Web Weather Indicator");
}

const long ThirtyMinutes = 1000 * 60 * 30 * 1000;
const long OneMinute = 1000 * 60;

long timeToWait;

void loop()
{
	timeToWait = checkWeather() ? ThirtyMinutes : OneMinute;
	delay(timeToWait);
}
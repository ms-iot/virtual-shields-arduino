#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <../VirtualShield/Screen.h>
#include <../VirtualShield/Web.h>
#include <../VirtualShield/Geolocator.h>

VirtualShield shield;
Screen screen = Screen(shield);
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

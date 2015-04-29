#ifndef Geolocator_h
#define Geolocator_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Geolocator : public Sensor {
public:
	double Latitude;
	double Longitude;
	double Altitude;

	Geolocator(const VirtualShield &shield);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
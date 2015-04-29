#ifndef Orientation_h
#define Orientation_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Orientation : public Sensor {
public:
	double X;
	double Y;
	double Z;
	double W;

	Orientation(const VirtualShield &shield);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
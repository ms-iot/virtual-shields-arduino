#ifndef Compass_h
#define Compass_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Compass : public Sensor {
public:
	double Heading;

	Compass(const VirtualShield &shield);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
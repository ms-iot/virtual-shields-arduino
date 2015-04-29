#ifndef Gyrometer_h
#define Gyrometer_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Gyrometer : public Sensor {
public:
	double X;
	double Y;
	double Z;

	Gyrometer(const VirtualShield &shield);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
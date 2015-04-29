#ifndef Accelerometer_h
#define Accelerometer_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Accelerometer : public Sensor {
public:
	double X;
	double Y;
	double Z;

	Accelerometer(const VirtualShield &shield);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
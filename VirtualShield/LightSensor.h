#ifndef LightSensor_h
#define LightSensor_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class LightSensor : public Sensor {
public:
	double Lux;

	LightSensor(const VirtualShield &shield);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
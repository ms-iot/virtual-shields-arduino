#ifndef Vibrate_h
#define Vibrate_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Vibrate : public Sensor {
public:
	Vibrate(const VirtualShield &shield);

	int vibrate(int milliseconds);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
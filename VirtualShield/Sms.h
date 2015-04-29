#ifndef Sms_h
#define Sms_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Sms : public Sensor {
public:
	Sms(const VirtualShield &shield);

	int send(String to, String message, String attachment = (const char*) 0);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
#ifndef Email_h
#define Email_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Email : public Sensor {
public:
	Email(const VirtualShield &shield);

	int send(String to, String subject, String message, String cc = (const char *)0, String attachment = (const char *)0);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
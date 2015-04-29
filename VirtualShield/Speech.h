#ifndef Speech_h
#define Speech_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

class Speech : public Sensor {
public:
	Speech(const VirtualShield &shield);

    bool IsSpeaking = false;

	int speak(String message);
	int stop() override;

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif

#ifndef Microphone_h
#define Microphone_h

#include "Sensor.h"

class Microphone : public Sensor
{
public:
	Microphone(const VirtualShield &shield);

	int record(long milliseconds, String url = (const char*)0, bool keepLocal = false, bool autoplay = false);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
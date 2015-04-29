
#ifndef Notification_h
#define Notification_h

#include "Sensor.h"

class Notification : public Sensor
{
public:
	Notification(const VirtualShield &shield);

	int toast(String message, String tag = (const char*)0, String image = (const char*)0, String audio = (const char*)0);
	int tile(String message, String tag = (const char*)0, String image = (const char*)0);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
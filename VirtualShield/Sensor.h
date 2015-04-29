#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

#include "SensorModels.h"
#include "VirtualShield.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>
#include "Attr.h"

class VirtualShield;

const PROGMEM char MESSAGE[] = "Message";
const PROGMEM char MS[] = "Ms";
const PROGMEM char TO[] = "To";
const PROGMEM char ATTACHMENT[] = "Attachment";
const PROGMEM char ENABLE[] = "ENABLE";
const PROGMEM char DISABLE[] = "DISABLE";
const PROGMEM char ACTION[] = "Action";
const PROGMEM char TAG[] = "Tag";
const PROGMEM char IMAGE[] = "IMAGE";
const PROGMEM char AUDIO[] = "Audio";
const PROGMEM char URL[] = "Url";
const PROGMEM char STOP[] = "STOP";

class Sensor {
public:
	void(*onEvent)(ShieldEvent* shieldEvent);

	VirtualShield& shield;
	ShieldEvent recentEvent;

	const char sensorType;
	bool isRunning = false;

	Sensor(const VirtualShield &shield, const char sensorType);

	int start(double delta = 0, long interval = 0);
	virtual int stop();
	int get();
	int getOnChange(double delta = 0);

	bool isUpdated();

	int writeAll(const char* serviceName, EPtr values[], int count, Attr extraAttributes[] = 0, int extraAttributeCount = 0);
	int sensorAction(SensorAction sensorAction, double delta = 0, long interval = 0) const;

	virtual bool isEvent(const char* tag, const char* action, ShieldEvent* shieldEvent);
	virtual bool isEvent(int id, const char* action, ShieldEvent* shieldEvent);

	void setOnEvent(void(*onEvent)(ShieldEvent* shieldEvent))
	{
		this->onEvent = onEvent;
	}

	int sendStop(const char* serviceName);

	virtual void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent);

protected:
	bool _isUpdated = false;
};

struct SensorEvent : ShieldEvent {
	SensorEvent();

	Sensor* sensor;
};

#endif 
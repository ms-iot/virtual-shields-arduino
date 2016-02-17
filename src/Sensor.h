/*
    Copyright(c) Microsoft Open Technologies, Inc. All rights reserved.

    The MIT License(MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef Sensor_h
#define Sensor_h

#include <ArduinoJson.h>

#include "Attr.h"
#include "SensorModels.h"
#include "ShieldEvent.h"
#include "VirtualShield.h"

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
    ShieldEvent::callback_t onEvent;

	VirtualShield& shield;
	ShieldEvent* recentEvent;

	const char sensorType;
	bool isRunning = false;

	Sensor(const VirtualShield &shield, const char sensorType);

	int start(double delta = 0, long interval = 0);
	virtual int stop();
	int get();
	int getOnChange(double delta = 0);

	bool isUpdated();

	int writeAll(const char* serviceName, EPtr values[], int count, Attr extraAttributes[] = NULL, int extraAttributeCount = 0);
	int sensorAction(SensorAction sensorAction, double delta = 0, long interval = 0) const;

	virtual bool isEvent(const char* tag, const char* action, ShieldEvent* shieldEvent);
	virtual bool isEvent(int id, const char* action, ShieldEvent* shieldEvent);

	void setOnEvent(ShieldEvent::callback_t onEvent)
	{
		this->onEvent = onEvent;
	}

	int sendStop(const char* serviceName);
    int filter(const char* serviceName, EPtr values[], int count);

	virtual void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent);

protected:
	bool _isUpdated = false;
};

struct SensorEvent : ShieldEvent {
	SensorEvent();

	Sensor* sensor;
};

#endif 